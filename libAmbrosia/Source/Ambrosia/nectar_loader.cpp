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
#include "nectar.h"
#include "status.h"
#include "target.h"

// C++ includes
#include <fstream>
    using std::ifstream;
#include <istream>
    using std::istream;
#include <iterator>
    using std::back_insert_iterator;
#include <memory>
    using std::unique_ptr;
/* <set> */
    using std::set;
/* <string> */
    using std::string;
#include <vector>
    using std::vector;

libambrosia_namespace_begin

const set<char> s_special_characters = { '(', ')', '{', '}', ':' };

nectar_loader::nectar_loader( const string &filename, istream &stream )
:   m_filename( filename ),
    m_stream( stream ),
    m_buffer(),
    m_token(),
    m_line_number(),
    m_global_processed( false )
{   }

template<class output_iterator>
void nectar_loader::extract_nectar( output_iterator it )
{
    while( next_token() )
    {
        debug() << "nectar_loader::processing token: \'" << m_token << "\'.\n";
        if( "global" == m_token )
        {
            debug() << "nectar_loader::global section found at line " << m_line_number << ".\n";
            if( m_global_processed )
                emit_error( "Second global section found in nectar file. Only one global section per *.nectar.txt file is allowed." );

            if( next_token() && "{" == m_token )
            {
            it = std::move( unique_ptr<target>( new target("global", target_type::global,
                                       string( ), m_line_number) ) );
            }
            else
                return syntax_error( "\'global\' must be followed by \'{\'." );
        }
        else if( "app" == m_token )
        {
            debug() << "nectar_loader::app section found at line " << m_line_number << ".\n";
        }
        else if( "lib" == m_token )
        {
            debug() << "nectar_loader::lib section found at line " << m_line_number << ".\n";
        }
        else if( "sub" == m_token )
        {
            debug() << "nectar_loader::sub section found at line " << m_line_number << ".\n";
            // get name and dependencies of sub target
            if( next_token() )
            {
                const string sub_directory = s_build_config.source_directory()
                                             + directory_seperator + m_token;
                string sub_file( m_token + ".nectar.txt" );
                string sub_project_file( sub_directory + directory_seperator + sub_file );
                if( !file_exists(sub_project_file) )
                {
                    debug() << "nectar_loader::sub target name and subproject file name do not match.\n";
                    sub_file = find_nectar_file( sub_directory );
                    if( libambrosia::current_status() == status::error )
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
}
template void nectar_loader::extract_nectar<back_insert_iterator<vector<unique_ptr<target> > > >( back_insert_iterator<vector<unique_ptr<target> > > );

void nectar_loader::syntax_error( const string &message ) const
{
    emit_error( "Syntax error (line " + to_string(m_line_number) + ") " + message );
}

bool nectar_loader::next_token()
{
    string token;
    if( fetch_token(token) )
    {
        m_token = token;
        return true;
    }
    else
        return false;
}

void nectar_loader::strip_comments( string &line )
{
    size_t index = line.find("#");
    if( index < string::npos )
    {
        line.resize( index ); // cut off comments
    }
}
bool nectar_loader::strip_newline_escape( string &line )
{
    const size_t index = line.find( "\\" );
    if( index > string::npos )
        return false;

    line.resize( index ); // cut off right before '\'
    return true;
}

bool nectar_loader::fetch_line()
{
    debug() << "nectar_loader::fetch_line called (line " << m_line_number+1 << ").\n";
    string line;
    if( std::getline(m_stream, line) )
    {
        ++m_line_number;
        strip_comments( line );
        if( line.empty() || line.find_first_not_of( " \v\t\r\n" ) == string::npos )
            return fetch_line();
        line = tokenize( line, s_special_characters );
        m_buffer << line;
        return true;
    }
    else
        return false;
}
bool nectar_loader::fetch_token( string &token )
{
    do // read new line until valid token can be extracted
    {
        if( m_buffer >> token )
        {
            debug() << "nectar_loader::Token extracted: " << token << "\n";
            m_token = token;
            return true; // return when token found
        }
        m_buffer.clear();
    } while( fetch_line() );
    // if no tokens can be extracted from the whole file, return false
    return false;
}
const string nectar_loader::tokenize( const string &line, const set<char> &special_characters )
{
    const auto not_found = special_characters.end();
    const auto end = line.end();
    string result;

    if( !line.empty() )
    {
        // copy first character
        result += line[0];

        char previous = line[0];
        for( auto it = line.begin()+1; it != end; ++it )
        {
            const char current = *it;

            if( special_characters.find(previous) != not_found )
                result += ' ';

            result += current;
            previous = current;
        }
    }
    return result;
}

libambrosia_namespace_end
