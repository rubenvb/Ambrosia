/**
  * Ambrosia/Parser/parser_state.h
  * Abstract parser class that provides solid tokenization and
  *  other useful functionality.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef PARSER_H
#define PARSER_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "state.h"

// C++ includes
#include <iosfwd>
#include <set>
#include <sstream>
#include <string>

libambrosia_namespace_begin

class parser_state : public state
{
public:
    parser_state( std::istream &stream, const size_t line_number, state* parent = 0 );
    virtual ~parser_state();

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

libambrosia_namespace_end

#endif // PARSER_H
