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

// C++ includes
#include <iosfwd>
/* <utility> */
/* <vector> */


libambrosia_namespace_begin

class nectar_loader
{
public:
    nectar_loader( const std::string &filename, std::istream &stream );

    template<class output_iterator>
    void extract_nectar( output_iterator it );

private:
    const std::string &m_filename;
    std::istream &m_stream;
    std::stringstream m_buffer;
    std::string m_token;
    size_t m_line_number;
    bool m_global_processed;
    // functions
    void syntax_error( const std::string &message ) const;
    bool next_token( std::string &token );
    // reads colon-lists of dependencies, ends at first '{'
    const std::vector<std::pair<target_type, std::string> > read_dependency_list();
    // finds matching curly brace and stores all stream contents in between in return value.
    const std::string read_code_block();
};

libambrosia_namespace_end

#endif // NECTAR_LOADER_H
