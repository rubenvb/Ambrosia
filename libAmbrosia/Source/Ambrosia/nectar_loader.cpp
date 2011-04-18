/**
  * Parser/nectar loader.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "nectar_loader.h"

// libAmbrosia includes
#include "algorithm.h"
#include "debug.h"
#include "global.h"
#include "enums.h"
#include "enum_maps.h"
#include "nectar.h"
#include "status.h"
#include "target.h"

// C++ includes
#include <fstream>
    using std::ifstream;
#include <ios>
    using std::noskipws;
#include <istream>
    using std::istream;
    using std::istream_iterator;
#include <iterator>
    using std::back_insert_iterator;
#include <locale>
    using std::isspace;
#include <memory>
    using std::unique_ptr;
/* <string> */
    using std::string;
/* <utility> */
    using std::pair;
#include <vector>
    using std::vector;

libambrosia_namespace_begin

nectar_loader::nectar_loader( const string &filename, istream &stream )
:   m_filename( filename ),
    m_stream( stream ),
    m_buffer(),
    m_token(),
    m_line_number( 1 ),
    m_global_processed( false )
{   }

template<class output_iterator>
void nectar_loader::extract_nectar( output_iterator it )
{
    debug() << "nectar_loader::Processing file: " << m_filename << ".\n";
    string token;
    while( next_token(token) )
    {
        debug() << "nectar_loader::processing token: \'" << token << "\'.\n";
        if( "global" == token )
        {
            debug() << "nectar_loader::global section found at line " << m_line_number << ".\n";
            if( m_global_processed )
                emit_error( "Second global section found in nectar file. Only one global section per *.nectar.txt file is allowed." );

            if( next_token(token) && "{" == token )
            {
            it = std::move( unique_ptr<target>( new target("global", target_type::global,
                                       string( ), m_line_number) ) );
            }
            else
                return syntax_error( "\'global\' must be followed by \'{\'." );
        }
        else if( "app" == token || "lib" == token )
        {
            const target_type type( (*target_type_map.find(token)).second );
            debug() << "nectar_loader::" << token << " section found at line " << m_line_number << ".\n";
            if( next_token(token) )
            {
                if( "{" == token )
                    return syntax_error( "Expected " + token + " name after " + token + "." );
                else
                {
                    debug() << "nectar_loader::Processing " + (*target_type_map_inverse.find(type)).second + ": " + token + ".\n";
                    const string name( token );
                    const auto dependencies = read_dependency_list();
                    if( status::error == current_status() )
                        return;

                    const string text( read_code_block() );
                    if( status::error == current_status() )
                        return;

                    it = std::move( unique_ptr<target>(new target(name, type, text, m_line_number)) );
                }
            }
        }
        else if( "sub" == token )
        {
            debug() << "nectar_loader::sub section found at line " << m_line_number << ".\n";
            // get name and dependencies of sub target
            if( next_token(token) )
            {
                const string sub_directory = s_build_config.source_directory()
                                             + directory_seperator + token;
                string sub_file( token + ".nectar.txt" );
                string sub_project_file( sub_directory + directory_seperator + sub_file );
                if( !file_exists(sub_project_file) )
                {
                    debug() << "nectar_loader::sub target name and subproject file name do not match.\n";
                    sub_file = find_nectar_file( sub_directory );
                    if( status::error == current_status() )
                        return; // no *.nectar.txt file found

                    debug() << "nectar_loader::found sub-.nectar.txt file: " << sub_file << ".\n";
                    sub_project_file = sub_directory + directory_seperator + sub_file;
                }
                debug() << "nectar_loader:: Opening subproject file: " << sub_project_file << ".\n";
                ifstream stream( sub_project_file );
                if( stream )
                {
                    nectar_loader sub_loader( sub_file, stream );
                    sub_loader.extract_nectar( it );
                    if( status::error == current_status() )
                        return;
                }
                else // opening file failed
                {
                    emit_error( "Error opening subproject file: " + sub_file + ". (line " + to_string(m_line_number) + ")" );
                    return;
                }
            }
            else
                return syntax_error( "\'sub\' must be followed by the name of the subproject." );
        }
        else
            return syntax_error( "Expected global, app, lib, or sub." );
    }
    debug() << "nectar_loader::Finished with file: " << m_filename << ".\n";
}
template void nectar_loader::extract_nectar<back_insert_iterator<vector<unique_ptr<target> > > >( back_insert_iterator<vector<unique_ptr<target> > > );

bool nectar_loader::next_token( string &token )
{
    // TODO: test the hell out of this function
    token.clear();
    char c;

    while( m_stream.get(c) )
    {
        debug() << "nectar_loader::next_token::line number " << m_line_number << ", character: \'" << c << "\', token so far: " << token << ".\n";
        if( token.empty() )
        {
            if( '\n' == c )
                ++m_line_number;
            else if( isspace(c,m_stream.getloc()) )
                continue;
            else if( '#' == c )
            {   // skip over preceding comments
                string temp;
                std::getline( m_stream, temp );
                ++m_line_number;
            }
            else if( contains(s_special_characters, c) )
            {   // special characters are tokens of their own
                token.append( 1, c );
                break;
            }
            else if( '\\' == c )
            {
                string temp;
                std::getline( m_stream, temp );
                ++m_line_number;
                continue;
            }
            else if( '\\' == c )
                goto newline_escape;
            else
                goto add_char;
        }
        else if( isspace(c, m_stream.getloc()) )
        {   // new whitespace == end of token
            m_stream.putback( c );
            break;
        }
        else if( '\\' == c )
        {   // newline escapes end current token
            newline_escape:
            string temp;
            std::getline( m_stream, temp );
            ++m_line_number;
            break;
        }
        else
            add_char:
            token.append( 1, c );
    }
    return !token.empty();
}

const vector<pair<target_type, string> > nectar_loader::read_dependency_list()
{
    vector<pair<target_type, string> > dependency_list;
    string token;
    while( next_token(token) )
    {
        if( "{" == token )
            return dependency_list;
        else if( ":" == token)
        {
            colon:
            if( next_token(token) )
            {
                const auto it = target_type_map.find(token);
                if( it !=  target_type_map.end()
                    && next_token(token) )
                {
                    if( "{" == token )
                        syntax_error( "Expected target name." );
                    else
                    {
                        do
                        {
                            dependency_list.push_back( {(*it).second, token} );
                            if( !next_token(token) )
                                syntax_error( "Expected \',\' or target name" );
                        } while( "," == token && next_token(token) );
                        if( ":" == token )
                            goto colon;
                    }
                }
                else
                    syntax_error( "Expected sub, app, or lib." );
            }
        }
        else
        {
            syntax_error( "expected begin of target body (\'{\') or dependency list (\':\' followed by a target type and target name)." );
            break;
        }
    }
    return dependency_list;
}

const std::string nectar_loader::read_code_block()
{
    string block;
    size_t curly_brace_count = 1; // stream pointer starts inside of curly brace block
    size_t parenthesis_count = 0;
    bool inside_quotes = false;

    noskipws(m_stream);

    char current;
    for( istream_iterator<char> it(m_stream);
         curly_brace_count > 0  && m_stream;
         ++it )
    {
        current = *it;
        debug() << "nectar_loader::read_code_block::Current character: \'" << current << "\'.\n";
        if( inside_quotes )
        {
            if( '\"' == current )
                inside_quotes = false;
        }
        else if( '\"' == current )
            inside_quotes = true;
        else if( '(' == current )
            ++parenthesis_count;
        else if( ')' == current )
        {
            if( parenthesis_count >= 1 )
                --parenthesis_count;
            else
            {
                syntax_error( "Too many closing parentheses \')\'." );
                break;
            }
        }
        else if( '{' == current )
            ++curly_brace_count;
        else if( '}' == current )
        {
            if( curly_brace_count >= 1 )
                --curly_brace_count;
            else
            {
                syntax_error( "Too many closing curly braces \'}\'." );
                break;
            }
        }
    }
    return block;
}

void nectar_loader::syntax_error( const string &message ) const
{
    emit_error( "Syntax error (line " + to_string(m_line_number) + ") " + message );
}

libambrosia_namespace_end
