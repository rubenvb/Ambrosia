/**
  * Parser/parser.h
  * Abstract parser interface class.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef PARSER_H
#define PARSER_H

// Global include
#include "global.h"

// C++ includes
#include <istream>
#include <string>

libambrosia_namespace_begin

// characters that are read as seperate tokens in Ambrosia .nectar.txt files.
extern const std::set<char> s_special_characters;
extern const std::set<char> s_special_characters_newline;

class parser
{
public:
    parser( const std::string &filename, std::istream &stream,
            const size_t line_number = 1 );
    virtual ~parser() = 0;

protected:
    const std::string &m_filename;
    std::istream &m_stream;
    size_t m_line_number;
    // Functions
    bool next_token( std::string &token, const std::set<char> &special_characters = s_special_characters );
    void syntax_error( const std::string &message ) const; // emit_error wrapper
    void syntax_warning( const std::string &message ) const; // emit_warning wrapper
};

libambrosia_namespace_end

#endif // _H

