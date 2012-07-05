/**
  * Ambrosia/Error/syntax_error.h
  * A syntax error.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_ERROR_SYNTAX_ERROR_H
#define AMBROSIA_ERROR_SYNTAX_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/nectar_error.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class syntax_error : public nectar_error
{
public:
  syntax_error(const std::string& message,
               const std::string& filename,
               const size_t line_number,
               const string_vector& list = {} );

  void output_message() const;
};

libambrosia_namespace_end

#endif // AMBROSIA_ERROR_SYNTAX_ERROR_H
