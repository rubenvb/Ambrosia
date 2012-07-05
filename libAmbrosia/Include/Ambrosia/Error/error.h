/**
  * Ambrosia/Error/error.h
  * Ambrosia's exception base class.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef AMBROSIA_ERROR_ERROR_H
#define AMBROSIA_ERROR_ERROR_H

// Global include
#include <Ambrosia/global.h>

// libAmbrosia includes
#include "Ambrosia/typedefs.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class error
{
public:
    error( const std::string& message, const string_vector& list = string_vector() );
    error( const std::string& message, const file_set& list );
    virtual ~error();

    virtual void output_message() const;

protected:
    const std::string m_message;
    string_vector m_list;
};

libambrosia_namespace_end

#endif // AMBROSIA_ERROR_ERROR_H
