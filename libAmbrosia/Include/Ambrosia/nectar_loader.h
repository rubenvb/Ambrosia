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
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Configuration/build_config.h"
#include "Ambrosia/nectar.h"

// C++ includes
#include <iosfwd>
/* <utility> */
/* <vector> */

libambrosia_namespace_begin

// characters that are read as seperate tokens in Ambrosia .nectar.txt files.
extern const std::set<char> s_special_characters;
extern const std::set<char> s_special_characters_newline;

// main nectar_loader class
class nectar_loader
{
public:
    nectar_loader( const std::string &filename, std::istream &stream,
                   const dependency_list &list = dependency_list() );
    ~nectar_loader();

    void extract_nectar( target_list &targets );

    // Disallow copy(constructor)ing and assignment (shuts up warning of -Weffc++)
    nectar_loader & operator=( const nectar_loader & ) = delete;
    nectar_loader( const nectar_loader & ) = delete;

private:
    const std::string &m_filename;
    std::istream &m_stream;
    size_t m_line_number;
    const dependency_list &m_dependency_list;
    bool m_global_processed;
    std::unique_ptr<target> p_target; // temporary pointer to current target
/*
 * emit_error and emit_warning wrappers
 ***************************************/
    void syntax_error( const std::string &message ) const;
    void syntax_warning( const std::string &message ) const;
/*
 * Token reading
 ****************/
    bool next_token( std::string &token, const std::set<char> &special_characters = s_special_characters );
    bool next_list_token( std::string &token );
    bool process_conditional();
    // reads colon-lists of dependencies, ends at first '{'
    void read_dependency_list( dependency_list &dependencies );
    // finds matching curly brace and stores all stream contents in between in return value.
    const std::string read_code_block();
/*
 * Parsing
 **********/
    // conditionals
    bool resolve_conditional( const std::function<bool(const std::string&)> &config_contains );
    bool process_outer_conditional();      // evaluated against s_ambrosia_config, skips full target
    bool process_dependency_list_conditional(); // evaluated against s_ambrosia_config, skips dependenc(y/ies)
    bool process_inner_conditional();      // evaluated against m_build_config, skips whole list
    bool process_inner_list_conditional(); // evaluated against m_build_config, skips item in list
    // item lists
    bool parse_file_list( const file_type type ); // matches wildcards to filenames and checks existence
    bool parse_source_directory_list( const file_type type ); // checks if directory exists
    bool parse_build_directory( const file_type type );  // will be created on first use
    bool parse_variable_list( string_set &items ); // only adds or removes strings from variables
    // main target parser
    void parse_target();
    // input validation functions (see wiki for valid input requirements)
    bool validate_variable( const std::string &config );
    bool validate_filename( const std::string &filename );
    bool validate_directory( const std::string &directory );
};

libambrosia_namespace_end

#endif // NECTAR_LOADER_H
