/**
  * Ambrosia/Error/internal_error.h
  * Internal logic error that prints a request for a bug report submission.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef INTERNAL_ERROR_H
#define INTERNAL_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/error.h"

libambrosia_namespace_begin

class internal_error : public error
{
public:
    internal_error( const std::string &message );

    void output_message() const;
};

libambrosia_namespace_end

#endif // INTERNAL_ERROR_H
