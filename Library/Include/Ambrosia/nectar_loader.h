/**
  * Parser/nectar loader.h
  * Class that extracts principal information from the project file:
  *  - canonical tokenization
  *  - target names
  *  - dependencies
  *  - TODO: user options
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef NECTAR_LOADER_H
#define NECTAR_LOADER_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "Parser/parser_state.h"

// C++ includes
#include <iosfwd>
/* <set> */

namespace ambrosia
{
    extern const std::set<char> s_special_characters;

    class nectar_loader
    {
    public:
        nectar_loader( std::istream &stream );

        template<class output_iterator>
        void extract_nectar( output_iterator it );

    private:
        std::istream &m_stream;
        std::stringstream m_buffer;
        std::string m_token;
        size_t m_line_number;
        // functions
        bool next_token();
        void strip_comments( std::string &line );
        bool strip_newline_escape( std::string &line );
        bool fetch_line();
        bool fetch_token( std::string &token );
        const std::string tokenize( const std::string &line, const std::set<char> &special_characters );
    };
} // namespace ambrosia

#endif // NECTAR_LOADER_H
