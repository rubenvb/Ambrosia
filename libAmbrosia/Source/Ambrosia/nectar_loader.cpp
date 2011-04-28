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
#include <string>
    using std::string;
/* <utility> */
    using std::pair;
#include <vector>
    using std::vector;

libambrosia_namespace_begin

nectar_loader::nectar_loader( const string &filename, istream &stream,
                              const dependency_list &list)
:   m_filename( filename ),
    m_stream( stream ),
    m_dependency_list( list ),
    m_line_number( 1 ),
    m_global_processed( false )
{   }

void nectar_loader::extract_nectar( target_list &targets )
{
    debug(2) << "nectar_loader::Processing file: " << m_filename << ".\n";
    string token;
    while( next_token(token) )
    {
        debug(3) << "nectar_loader::processing token: \'" << token << "\'.\n";
        if( "global" == token )
        {
            debug(4) << "nectar_loader::global section found at line " << m_line_number << ".\n";
            if( m_global_processed )
                emit_error( "Second global section found in nectar file. Only one global section per *.nectar.txt file is allowed." );

            if( next_token(token) && "{" == token )
            {
                targets.push_back( std::move(unique_ptr<target>(new target("global", target_type::global, dependency_list(),
                                                                           read_code_block(), m_line_number))) );
            }
            else
                return syntax_error( "\'global\' must be followed by \'{\'." );
        }
        else if( "app" == token || "lib" == token )
        {
            const target_type type( (*target_type_map.find(token)).second );
            debug(4) << "nectar_loader::" << token << " section found at line " << m_line_number << ".\n";
            if( next_token(token) )
            {
                if( "{" == token )
                    return syntax_error( "Expected " + token + " name after " + token + "." );
                else
                {
                    debug(4) << "nectar_loader::Processing "
                             << map_value(target_type_map_inverse, type) << ": " << token << ".\n";
                    const string name( token );
                    dependency_list dependencies( m_dependency_list );
                    read_dependency_list( dependencies );
                    if( error_status() )
                        return;

                    const string text( read_code_block() );
                    if( error_status() )
                        return;

                    targets.push_back( std::move(unique_ptr<target>(new target(name, type, dependencies,
                                                                               text, m_line_number))) );
                }
            }
        }
        else if( "sub" == token )
        {
            debug(4) << "nectar_loader::sub section found at line " << m_line_number << ".\n";
            // get name and dependencies of sub target
            if( next_token(token) )
            {
                const string sub_directory = s_build_config.source_directory()
                                             + directory_seperator + token;
                string sub_file( token + ".nectar.txt" );
                string sub_project_file( sub_directory + directory_seperator + sub_file );
                if( !file_exists(sub_project_file) )
                {
                    debug(4) << "nectar_loader::sub target name and subproject file name do not match.\n";
                    sub_file = find_nectar_file( sub_directory );
                    if( error_status() )
                        return; // no *.nectar.txt file found

                    debug(4) << "nectar_loader::found sub-.nectar.txt file: " << sub_file << ".\n";
                    sub_project_file = sub_directory + directory_seperator + sub_file;
                }
                debug(4) << "nectar_loader:: Opening subproject file: " << sub_project_file << ".\n";
                ifstream stream( sub_project_file );
                if( stream )
                {
                    dependency_list dependencies;
                    read_dependency_list( dependencies );
                    if( error_status() )
                        return;

                    nectar_loader sub_loader( sub_project_file, stream, dependencies );
                    sub_loader.extract_nectar( targets );
                    if( error_status() )
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
    debug(3) << "nectar_loader::Finished with file: " << m_filename << ".\n";
}
//template void nectar_loader::extract_nectar<back_insert_iterator<target_list>( back_insert_iterator<target_list> );

bool nectar_loader::next_token( string &token, const std::set<char> &special_characters )
{
    // TODO: test the hell out of this function
    token.clear();
    char c;

    while( m_stream.get(c) )
    {
        debug(7) << "nectar_loader::next_token::line number " << m_line_number << ", character: \'" << c << "\', token so far: " << token << "\n";
        if( token.empty() )
        {
            if( '\n' == c )
                ++m_line_number;

            if( contains(special_characters, c) )
            {   // special characters are tokens of their own
                debug(6) << "nectar_loader::next_token::Detected special character.\n";
                token.append( 1, c );
                return true;
            }
            else if( isspace(c, m_stream.getloc()) )
                continue;
            else if( '#' == c )
            {   // skip over preceding comments
                string temp;
                std::getline( m_stream, temp );
                ++m_line_number;
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
    debug(7) << "nectar_loader::next_token:Token extracted: " << token << ".\n";
    return !token.empty();
}

void nectar_loader::read_dependency_list( dependency_list &dependencies )
{
    // copy "parent" dependencies
    dependencies = m_dependency_list;
    debug(4) << "nectar_loader::read_dependency_list::Reading dependencies.\n";
    bool in_list = false;
    target_type type;
    string token;
    while( next_token(token, s_special_characters_newline) )
    {
        debug(6) << "nectar_loader::read_dependency_list::token: " << token << ".\n";
        if( "{" == token || "\n" == token )
        {
            debug(4) << "nectar_loader::read_dependency_list::" << dependencies.size() << " dependencies.\n";
            return;
        }
        else if( !in_list )
        {
            if( ":" == token)
            {
                if( next_token(token) )
                {
                    if( "lib"==token )
                        type = target_type::lib;
                    else if( "app" == token )
                        type = target_type::app;

                    debug(6) << "nectar_loader::read_dependency_list::Detected " << token << " dependencies.\n";
                    in_list = true;
                    goto insert_dependency;
                }
                else
                    return syntax_error( "Expected list of target names after \':\'." );
            }
            else
                return syntax_error( "Expected target body \'{\' or dependency list \':\'." );
        }
        else if( "," == token )
        {
            insert_dependency:
            if( next_token(token) )
            {

                debug(6) << "nectar_loader::read_dependency_list::Inserting dependency: " << token << ".\n";
                if( !dependencies.insert({type, token}).second )
                    return syntax_error( "Double dependency listed: "
                                         + map_value(target_type_map_inverse, type) + " " + token + "."  );
            }
        }
        else
            in_list = false;
    }
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
        block.append( 1, current );
        debug(9) << "nectar_loader::read_code_block::Current character: \'" << current << "\'.\n";
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
    emit_error( "Syntax error: " + m_filename + ": line " + to_string(m_line_number) + "\n\t" + message );
}

libambrosia_namespace_end
