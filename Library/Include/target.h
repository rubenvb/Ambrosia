/**
  * target.h
  * Ambrosia's internal representation of a build target. This includes:
  *  - full nectar.txt target text for:
  *     > multiple builds in one
  *     > decent error reporting referring to line number and text
  *  - full source file list (including headers etc.)
  *  - inter-target dependencies
  *  - ...
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef TARGET_H
#define TARGET_H

// Global include
#include "Ambrosia/Global.h"

// C++ includes
#include <string>

namespace ambrosia
{
    class target
    {
    public:
        target( const size_t line_number, const std::string &text );

        // mighty compiler/linker option duplicate filter functions
        static const std::string filter_duplicate_CFLAGS( const std::string &new_options,
                                                          const std::string previous_options );
        static const std::string filter_duplicate_LFLAGS( const std::string &new_options,
                                                          const std::string previous_options );

    private:
        // full target text, with beginning line number
        const size_t m_line_number;
        const std::string m_text;
    };
} // namespace ambrosia

#endif // TARGET_H
