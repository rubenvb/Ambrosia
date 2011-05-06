/**
  * Parser/target_parser.h
  * Parses target contents.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef TARGET_PARSER_H
#define TARGET_PARSER_H

// Global include
#include "global.h"

// libAmbrosia
#include "Parser/parser.h"

// C++ includes
/* <istream> */

// Forward declarations
class target;

libambrosia_namespace_begin

class target_parser : public parser
{
public:
    target_parser( target &t, std::istream &stream );

    void parse();

private:
    target &m_target;
};

libambrosia_namespace_end

#endif // TARGET_PARSER_H
