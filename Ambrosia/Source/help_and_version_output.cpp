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
#include "Ambrosia/enum_maps.h"
#include "Ambrosia/platform.h"

// Ambrosia includes
#include "output.h"

ambrosia_namespace_begin

void print_help_information()
{
    output() << "TODO: help output.\n";
}
void print_version_information()
{
    // Welcome message
    output() << "Welcome to Ambrosia, version "
               << app::version_major << "."
               << app::version_minor << "."
               << app::version_bugfix << ", using libAmbrosia version "
               << lib::version_major << "."
               << lib::version_minor << "."
               << lib::version_bugfix << ".\n"
               << "This version of Ambrosia was built for "
               << (*lib::os_map_inverse.find(lib::build_os)).second << " "
               << (*lib::architecture_map_inverse.find(lib::build_architecture)).second << ".\n";
}

ambrosia_namespace_end
