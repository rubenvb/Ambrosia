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

namespace ambrosia
{
    class target
    {
    public:
        target();

    private:
        bool m_parsed; // true if text contents have been converted to internal representation.
    };
} // namespace ambrosia

#endif // TARGET_H
