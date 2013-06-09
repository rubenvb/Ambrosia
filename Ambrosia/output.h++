/**
 *
 * Project Ambrosia: Ambrosia commandline build tool
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
 * output.h++
 * Class designed to be used as a temporary output stream.
 *
 **/

#ifndef OUTPUT_H
#define OUTPUT_H

// Global include
#include "global.h++"

// C++ includes
#include <iostream>

namespace ambrosia
{

class output
{
public:
  template<typename T>
  output& operator<<(const T& output)
  {
    output_stream << output;
    return *this;
  }
  // for std::endl and other manipulators
  typedef std::ostream& (*STRFUNC)(std::ostream&);
  output& operator<<(STRFUNC func)
  {
    func(output_stream);
    return *this;
  }
private:
  static std::ostream& output_stream;
};

} // namespace ambrosia

#endif // OUTPUT_H
