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

// C++ includes
#include <string>

libambrosia_namespace_begin

class target
{
public:
    target( const std::string &name, const target_type type,
            const std::string & text, const size_t line_number );

private:
    bool m_parsed; // true if text contents have been converted to internal representation.
    const std::string m_name;
    const target_type m_type;
    const std::string m_text; // unmodified target text without outer braces
    const size_t m_line_number; // line number of first line in m_text in the *.nectar.txt file
};

libambrosia_namespace_end

#endif // TARGET_H
