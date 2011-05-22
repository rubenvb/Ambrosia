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
    // TODO: test the *full* hell out of this function
    token.clear();
    bool inside_quotes = false;
    char c;

    while( m_stream.get(c) )
    {
        debug(7) << "nectar_loader::next_token::line number " << m_line_number << ", character: \'" << c << "\', token so far: " << token << "\n";
        if( inside_quotes )
        {
            if( '\"' == c )
                break; // end of token at end of quotes
            else if( '\n' == c )
            {
                syntax_error( "Quoted strings cannot span several lines." );
                return false;
            }
            else if( token.empty() && isspace(c, m_stream.getloc()) )
                syntax_error( "Beginning quote must not be followed by a whitespace." );
            else
                goto add_char;
        }
        else
        {
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
                else if( '\"' == c )
                {
                    inside_quotes = true;
                    continue;
                }
                else if( isspace(c, m_stream.getloc()) )
                    continue;
                else if( '#' == c )
                {   // skip over comments
                    string temp;
                    std::getline( m_stream, temp );
                    ++m_line_number;
                }
                else if( '(' == c )
                    process_conditional();
                else if( '\\' == c )
                {
                    string temp;
                    std::getline( m_stream, temp );
                    ++m_line_number;
                }
                else
                    goto add_char;
            }
            else if( isspace(c, m_stream.getloc()) )
            {   // new whitespace == end of token
                m_stream.putback( c );
                break;
            }
            else if( '\"' == c )
            {
                syntax_error( "Beginning quotes must be preceded by a whitespace or a special character." );
                return false;
            }
            else
                add_char:
                token.append( 1, c );
        }
    }
    debug(6) << "nectar_loader::next_token:Token extracted: \'" << token << "\'\n";
    return !token.empty();
}
bool parser::process_conditional()
{
    emit_warning( "Conditionals not implemented yet." );
    return true;
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
