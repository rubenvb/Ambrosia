/**
  * Ambrosia/Error/commandline_error.h
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "Ambrosia/Error/commandline_error.h"

// libAmbrosia includes
#include "Ambrosia/algorithm.h"

// C++ includes
#include <iostream>
  using std::cerr;
#include <string>
  using std::string;

libambrosia_namespace_begin

commandline_error::commandline_error(const string& message,
                                     const size_t argument_number)
: error(message),
  m_argument_number(argument_number)
{   }

void commandline_error::output_message() const
{
  cerr << "Commandline error: argument " << to_string(m_argument_number) << ":\n";
  error::output_message();
}

libambrosia_namespace_end
