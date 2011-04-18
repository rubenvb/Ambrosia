/**
  * target.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "target.h"

// Ambrosia includes
#include "debug.h"

// C++ includes
/* <string> */
    using std::string;

libambrosia_namespace_begin

target::target( const string &name, const target_type type,
                const string & text,
                const size_t line_number, const size_t column_number )
:   m_parsed( false ),
    m_name( name ),
    m_type( type ),
    m_text( text ),
    m_line_number( line_number ),
    m_column_number( column_number )
{   }

/*bool target::next_token()
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

void target::strip_comments( string &line )
{
    size_t index = line.find("#");
    if( index < string::npos )
    {
        line.resize( index ); // cut off comments
    }
}
bool target::strip_newline_escape( string &line )
{
    const size_t index = line.find( "\\" );
    if( index > string::npos )
        return false;

    line.resize( index ); // cut off right before '\'
    return true;
}

bool target::fetch_line()
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
bool target::fetch_token( string &token )
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
}*/


libambrosia_namespace_end
