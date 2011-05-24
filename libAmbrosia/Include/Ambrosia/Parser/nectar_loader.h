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

    // disallow copy(constructor)ing and assignment (shuts up warning of -Weffc++)
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
/*
 * Parsing
 **********/
    // conditionals
    bool resolve_conditional( const build_config& config ); //
    void process_outer_conditional();      // evaluated against s_build_config, skips full target
    void process_dependency_list_conditional(); // evaluated against s_build_config, skips dependenc(y/ies)
    void process_inner_conditional();      // evaluated against m_build_config, skips whole list
    void process_inner_list_conditional(); // evaluated against m_build_config, skips item in list
    // item list
    bool parse_list( std::function<bool(const std::string &)> insert,
                     std::function<bool(const std::string &)> remove ); // helper function to read item lists
    // main target parsers
    void parse_binary_or_global();
    void parse_install();
    void parse_test();
};

libambrosia_namespace_end

#endif // NECTAR_LOADER_H
