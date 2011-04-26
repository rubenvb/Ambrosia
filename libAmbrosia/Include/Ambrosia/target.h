/**
  * Ambrosia/target.h
  * Ambrosia's internal representation of a build target. This includes:
  *  - full nectar.txt target text for:
  *     > multiple builds in one
  *     > decent error reporting referring to line number and text
  *  - full source file list (including headers etc.)
  *  - inter-target dependencies
  *  - ...

  * Author: Ruben Van Boxem
  *
  **/

#ifndef TARGET_H
#define TARGET_H

// Global include
#include "global.h"

// libAmbrosia includes
#include "enums.h"
#include "nectar.h"
#include "node.h"

// C++ includes
/* <string> */
/* <utility> */
/* <vector> */

libambrosia_namespace_begin

class target : public node
{
public:
    target( const std::string &name, const target_type type,
            const dependency_list &dependencies, const std::string & text,
            const size_t line_number, const size_t column_number = 0 );

    // Getters
    const std::string & name() const;
    const target_type & type() const;
    const dependency_list & dependencies() const;

    // Comparison operator for std::set
    bool operator<( const target &t );

private:
    const target_type m_type; // target type
    const dependency_list m_dependencies;
    const std::string m_text; // unmodified target text without outer braces
    const size_t m_line_number; // line number of first line in m_text in the *.nectar.txt file
    const size_t m_column_number; // column number where the *first* line starts
    bool m_parsed; // true if text contents have been converted to internal representation.
    // PARSING TODO
 /*   std::string m_token;
    std::istream m_stream;
    bool next_token();
    void strip_comments( std::string &line );
    bool strip_newline_escape( std::string &line );
    bool fetch_line();
    bool fetch_token( std::string &token );*/
};

libambrosia_namespace_end

#endif // TARGET_H
