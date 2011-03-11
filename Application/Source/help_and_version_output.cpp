/**
  * help_and_version_output.cpp
  * Function implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "help_and_version_output.h"

// libAmbrosia includes
#include "Ambrosia/global.h"

// Ambrosia includes
#include "output.h"

namespace ambrosia
{
    void print_help_information()
    {
        output() << "TODO: help output.\n";
    }
    void print_version_information()
    {
        // Welcome message
        output() << "Welcome to Ambrosia, version "
                   << app_version_major << "."
                   << app_version_minor << "."
                   << app_version_bugfix << ", using libAmbrosia version "
                   << lib_version_major << "."
                   << lib_version_minor << "."
                   << lib_version_bugfix << ".\n"
                   << "This version of Ambrosia was built for TODO OS, Platform, architecture.\n";
    }
} // namespace ambrosia
