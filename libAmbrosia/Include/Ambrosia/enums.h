/**
  * Ambrosia/enums.h
  * Ambrosia's handy enums.
  *
  * Author: Ruben Van Boxem
  *
  **/

#ifndef ENUMS_H
#define ENUMS_H

// Global include
#include "Ambrosia/global.h"

ambrosia_namespace_begin

enum class os
{
    Windows,
    Linux,
    MacOSX
    // ...
};
enum class architecture
{
    x86,
    amd64
    //...
};
enum class toolchain
{
    GNU,
    Microsoft,
    LLVM,
    Intel
    // ...
};
enum class conditional_operator
{   // defines operator precedence
    right_parenthesis, // )
    left_parenthesis,  // (
    plus_op,           // +
    or_op,             // |
    not_op             // !
};
enum class target_type
{
    global,
    app,
    lib,
    sub,
    install,
    test
};
enum class file_type
{
    source,
    source_c,
    source_cxx,
    source_java,
    source_fortran,
    header,
    resource,
    Qt_ui,
    Qt_moc,
    Qt_rc
    // ...
};
inline file_type get_general_type( const file_type type )
{
    switch( type )
    {
        case file_type::source:
        case file_type::source_c:
        case file_type::source_cxx:
        case file_type::source_java:
        case file_type::source_fortran:
            return file_type::source;
        default:
            return type;
    }
}


ambrosia_namespace_end

#endif // ENUMS_H
