/**
  * Ambrosia/Error/commandline_error.h
  * Commandline errors or errors flowing forth directly from commandline errors.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_ERROR_COMMANDLINE_ERROR_H
#define AMBROSIA_ERROR_COMMANDLINE_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/error.h"

// C++ includes
/* <string> */

libambrosia_namespace_begin

class commandline_error : public error
{
public:
    commandline_error( const std::string &message, const size_t argument_number );

    void output_message() const;

private:
    const size_t m_argument_number;
};

libambrosia_namespace_end

#endif // AMBROSIA_ERROR_COMMANDLINE_ERROR_H
