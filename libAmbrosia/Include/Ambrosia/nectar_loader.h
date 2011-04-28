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
#include "build_config.h"
#include "Parser/parser_state.h"
#include "nectar.h"

// C++ includes
#include <iosfwd>
/* <utility> */
/* <vector> */

libambrosia_namespace_begin

class nectar_loader
{
public:
    nectar_loader( const std::string &filename, std::istream &stream,
                   const dependency_list &list = dependency_list() );

    void extract_nectar( target_list &targets );

private:
    const std::string &m_filename;
    std::istream &m_stream;
    const dependency_list &m_dependency_list;
    size_t m_line_number;
    bool m_global_processed;
    // functions
    bool next_token( std::string &token, const std::set<char> &special_characters = s_special_characters );
    // reads colon-lists of dependencies, ends at first '{'
    void read_dependency_list( dependency_list &dependencies );
    // finds matching curly brace and stores all stream contents in between in return value.
    const std::string read_code_block();
    // better error/warning reporting
    void syntax_error( const std::string &message ) const;
    void syntax_warning( const std::string &message ) const;
};

libambrosia_namespace_end

#endif // NECTAR_LOADER_H
