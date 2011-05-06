/**
  * Parser/parser.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Parser/parser.h"

// libAmbrosia includes
#include "algorithm.h"
#include "debug.h"
#include "status.h"

// C++ includes
/* <istream> */
    using std::istream;
/* <set> */
    using std::set;
/* <string> */
    using std::string;

libambrosia_namespace_begin

const set<char> s_special_characters = { '(', ')', '{', '}', ':', ',' };
const set<char> s_special_characters_newline = { '(', ')', '{', '}', ':', ',', '\n' };

parser::parser( const string &filename, istream &stream,
                const size_t line_number )
:   m_filename( filename ),
    m_stream( stream ),
    m_line_number( line_number )
{   }

parser::~parser()
{   }

bool parser::next_token( string &token, const std::set<char> &special_characters )
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


void parser::syntax_error( const string &message ) const
{
    emit_error( "Syntax error: " + m_filename + ": line " + to_string(m_line_number) + "\n\t" + message );
}
void parser::syntax_warning( const string &message ) const
{
    emit_warning( "Syntax warning: " + m_filename + ": line " + to_string(m_line_number) + ": " + message );
}

libambrosia_namespace_end
