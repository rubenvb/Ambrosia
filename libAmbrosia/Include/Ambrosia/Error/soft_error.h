/**
  * Ambrosia/Error/soft_error.h
  * I have no clue.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_ERROR_SOFT_ERROR_H
#define AMBROSIA_ERROR_SOFT_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/error.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class soft_error : public error
{
public:
  soft_error(const std::string& message);
  ~soft_error();
};

libambrosia_namespace_end

#endif // AMBROSIA_ERROR_SOFT_ERROR_H
