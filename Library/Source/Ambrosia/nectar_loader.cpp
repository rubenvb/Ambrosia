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
    :   m_stream( stream ),
        m_buffer(),
        m_token(),
        m_line_number()
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
            }

        }


    }
    template void nectar_loader::extract_nectar<back_insert_iterator<vector<target> > >( back_insert_iterator<vector<target> > );

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
            //m_comment = line.substr( index, string::npos ); // save comments on current line for an unknown reason :)
            line.resize( index ); // cut off comments
        }
    }
    bool nectar_loader::strip_newline_escape( string &line )
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

    bool nectar_loader::fetch_line()
    {
        debug() << "fetchLine called (line " << m_line_number+1 << ").\n";
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
                debug() << "Token extracted: " << token << "\n";
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
} // namespace ambrosia
