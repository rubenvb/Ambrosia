/**
  * Parser/nectar_loader.h
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
#include "Parser/parser.h"
#include "nectar.h"

// C++ includes
#include <iosfwd>
/* <utility> */
/* <vector> */

libambrosia_namespace_begin

class nectar_loader : public parser
{
public:
    nectar_loader( const std::string &filename, std::istream &stream,
                   const dependency_list &list = dependency_list() );
    ~nectar_loader();

    void extract_nectar( target_list &targets );

    // disallow copy(constructor)ing and assignment
    nectar_loader & operator=( const nectar_loader & ) = delete;
    nectar_loader( const nectar_loader & ) = delete;

private:
    const dependency_list &m_dependency_list;
    bool m_global_processed;
    std::unique_ptr<target> p_target; // temporary pointer to current target
    // functions
    // reads colon-lists of dependencies, ends at first '{'
    void read_dependency_list( dependency_list &dependencies );
    // finds matching curly brace and stores all stream contents in between in return value.
    const std::string read_code_block();
    // Parsing
    bool parse_list( std::pair<string_set, string_set> &items ); // helper function to read item lists
    void parse_global();
    void parse_binary();
    void parse_install();
    void parse_test();
};

libambrosia_namespace_end

#endif // NECTAR_LOADER_H
