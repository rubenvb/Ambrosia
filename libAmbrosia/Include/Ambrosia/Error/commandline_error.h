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
 * Ambrosia/Error/commandline_error.h
 * Commandline errors or errors flowing forth directly from commandline errors.
 *
 **/

#ifndef AMBROSIA_ERROR_COMMANDLINE_ERROR_H
#define AMBROSIA_ERROR_COMMANDLINE_ERROR_H

// Global include
#include "Ambrosia/global.h"

// libAmbrosia includes
#include "Ambrosia/Error/error.h"

// C++ includes
#include <string>

libambrosia_namespace_begin

class commandline_error : public error
{
public:
    commandline_error( const std::string& message, const size_t argument_number );

    void output_message() const;

private:
    const size_t m_argument_number;
};

libambrosia_namespace_end

#endif // AMBROSIA_ERROR_COMMANDLINE_ERROR_H
