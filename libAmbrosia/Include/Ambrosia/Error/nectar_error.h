/**
  * Ambrosia/Error/nectar_error.h
  * Error related to a *.nectar.txt file.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_ERROR_NECTAR_ERROR_H
#define AMBROSIA_ERROR_NECTAR_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/error.h"

// C++ includes
/* <string> */

libambrosia_namespace_begin

class nectar_error : public error
{
public:
    nectar_error( const std::string& message, const std::string& filename,
                  const size_t line_number, const string_vector& list = string_vector() );

    virtual void output_message() const;

protected:
    const std::string m_filename;
    const size_t m_line_number;
};

libambrosia_namespace_end

#endif // AMBROSIA_ERROR_NECTAR_ERROR_H
