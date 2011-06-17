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
#include "nectar.h"

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
    // static members for error reporting outside of nectar_loader
    static void make_current( const std::string* filename, const size_t* line_number );
    static const std::string* s_filename;
    static const size_t* s_line_number;
/*
 * Token reading
 ****************/
    bool next_token( std::string &token, const std::set<char> &special_characters = s_special_characters );
    bool process_conditional();
    static void syntax_error( const std::string &message ); // emit_error wrapper
    static void syntax_warning( const std::string &message ); // emit_warning wrapper
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
    // item list
    bool parse_list( std::function<bool(const std::string &)> validate,
                     std::function<bool(const std::string &)> insert,
                     std::function<bool(const std::string &)> remove ); // helper function to read item lists
    bool add_file( const file_type type, const std::string &filename );
    // main target parser
    void parse_target();
    // input validation functions (see wiki for valid input requirements)
    static bool validate_CONFIG( const std::string &config );
    static bool validate_filename( const std::string &filename );
    static bool validate_directory( const std::string &directory );
};

libambrosia_namespace_end

#endif // NECTAR_LOADER_H
