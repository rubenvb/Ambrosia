/**
 *
 * Project Ambrosia: Ambrosia library
 *
 * Written in 2012 by Ruben Van Boxem <vanboxem.ruben@gmail.com>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright and related
 * and neighboring rights to this software to the public domain worldwide. This software is
 * distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along with this software.
 * If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * Ambrosia/Error/error.h
 * Ambrosia's exception base class.
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
    error(const std::string& message,
          const string_vector& list = string_vector());
    error(const std::string& message,
          const file_set& list);
    virtual ~error();

    virtual void output_message() const;

protected:
    const std::string message;
    string_vector list;
};

libambrosia_namespace_end

#endif // AMBROSIA_ERROR_ERROR_H
