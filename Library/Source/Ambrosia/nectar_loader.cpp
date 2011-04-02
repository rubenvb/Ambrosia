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
#include "debug.h"
#include "target.h"

// C++ includes
#include <istream>
    using std::istream;
#include <iterator>
    using std::back_insert_iterator;
/* <set> */
    using std::set;
/* <string> */
    using std::string;
#include <vector>
    using std::vector;

namespace ambrosia
{
    const set<char> s_special_characters = { '(', ')', '{', '}', ':' };

    nectar_loader::nectar_loader( istream &stream )
    :   m_stream( stream )
    {   }

    template<class output_iterator>
    void nectar_loader::extract_nectar( output_iterator it )
    {


    }
    template void nectar_loader::extract_nectar<back_insert_iterator<vector<target> > >( back_insert_iterator<vector<target> > );

    bool parser_state::next_token()
    {
        string token;
        if( fetch_token(token) )
        {
            if( "(" == token )
            {
                debug() << "Conditionals not implemented yet.\n";
                return false;
            }
            else
            {
                m_token = token;
                return true;
            }
        }
        else
            return false;
    }

    void parser_state::strip_comments( string &line )
    {
        size_t index = line.find("#");
        if( index < string::npos )
        {
            m_comment = line.substr( index, string::npos ); // save comments on current line for an unknown reason :)
            line.resize( index ); // cut off comments
        }
    }
    bool parser_state::strip_newline_escape( string &line )
    {
        const size_t index = line.find( "\\" );
        if( index < string::npos )
        {
            line.resize( index ); // cut off right before '\'
            return true;
        }
        else
            return false;
    }

    bool parser_state::fetch_line()
    {
        debug() << "fetchLine called (line " << m_line_number+1 << ").\n";
        string line;
        if( std::getline(m_stream, line) )
        {
            ++m_line_number;
            strip_comments( line );
            if( line.empty() || line.find_first_not_of( " \v\t\r\n" ) == string::npos )
                return fetch_line();
            tokenize( line, s_special_characters );
            m_buffer << line;
            return true;
        }
        else
            return false;
    }
    bool parser_state::fetch_token( string &token )
    {
        do // read new line until valid token can be extracted
        {
            if( m_buffer >> token )
            {
                debug() << "Token extracted: " << token << "\n";
                m_token = token;
                return true; // return when token found
            }
            m_buffer.clear();
        } while( fetch_line() );
        // if no tokens can be extracted from the whole file, return false
        return false;
    }
    void parser_state::tokenize( string &line, const set<char> &special_characters )
    {
        auto it = line.begin();
        const auto not_found = special_characters.end();

        // first character special case
        if( special_characters.find( *it ) != not_found )
            it = line.insert( it+1, ' ' ) + 1;

        while( it != line.end() )
        {
            // check if we're dealing with a special character
            if( special_characters.find(*it) != not_found )
            {
                // ensure a space before
                if( *(it-1) != ' ' )
                    it = line.insert( it, ' ' ) + 1;
                // ensure a space after
                if( (it+1) != line.end() && *(it+1) != ' ' )
                    it = line.insert( it+1, ' ');
                else
                    line.append(" ");
            }
            ++it;
        }
    }

} // namespace ambrosia
