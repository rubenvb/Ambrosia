/**
  * parser.H
  * Abstract parser class that provides solid tokenization and
  *  other useful functionality.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef PARSER_H
#define PARSER_H

// Global include
#include "Ambrosia/global.h"

// C++ includes
#include <iosfwd>
#include <set>
#include <sstream>
#include <string>

namespace ambrosia
{
    extern const std::set<char> s_special_characters;

    class parser
    {
    public:
        parser( std::istream &stream, const size_t line_number );
        virtual ~parser() = 0; // pure virtual to make class abstract

    protected:
        std::string m_comment;
        std::string m_token;
        size_t m_line_number;
        // functions
        bool next_token();

    private:
        std::istream &m_stream;
        std::stringstream m_buffer;
        // functions
        void strip_comments( std::string &line );
        bool strip_newline_escape( std::string &line );
        bool fetch_line();
        bool fetch_token( std::string &token );
        void tokenize( std::string &line, const std::set<char> &special_characters );
    };
} // namespace ambrosia

#endif // PARSER_H
