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
 * Ambrosia/Error/internal_error.h
 * Internal logic error that prints a request for a bug report submission.
 *
 **/

#ifndef INTERNAL_ERROR_H
#define INTERNAL_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/error.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class internal_error : public error
{
public:
    internal_error( const std::string& message );

    void output_message() const;
};

libambrosia_namespace_end

#endif // INTERNAL_ERROR_H
