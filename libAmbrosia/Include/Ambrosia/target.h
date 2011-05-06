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
    target( const std::string &filename, const std::string &name,
            const target_type type, const dependency_list &dependencies,
            const std::string & text, const size_t line_number );

    // Getters
    const std::string & name() const;
    const target_type & type() const;
    const dependency_list & dependencies() const;
    const std::string &filename() const;
    const std::string &text() const;
    size_t line_number() const;
    bool is_parsed() const;

private:
    target_type m_type; // target type
    dependency_list m_dependencies;
    std::string m_filename;
    std::string m_text; // unmodified target text without outer braces
    size_t m_line_number; // line number of first line in m_text in the *.nectar.txt file
    bool m_parsed; // true if text contents have been converted to internal representation.
};

libambrosia_namespace_end

#endif // TARGET_H
