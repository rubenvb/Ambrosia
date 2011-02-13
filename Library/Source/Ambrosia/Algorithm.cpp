/**
  * Algorithm.cpp
  * Function implementations.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Function include
#include "Algorithm.h"

// C++ includes
/* <string> */
    using std::string;

namespace ambrosia
{

bool wildcardDirectoryCompare( const string &wildString, const string &fullString )
{
    // Written by Jack Handy - jakkhandy@hotmail.com
    // Taken from http://www.codeproject.com/KB/string/wildcmp.aspx
    // Adapted by Ruben Van Boxem for Ambrosia

    auto wild = wildString.begin();
    auto str = fullString.begin();
    const auto wildEnd = wildString.end();
    const auto strEnd = fullString.end();

    auto cp = strEnd;
    auto mp = wildEnd;

    while( str != strEnd && (*wild != '*') )
    {
        if( (*wild != *str) && (*wild != '?') )
            return false;

        ++wild;
        ++str;
    }
    while( str != strEnd ) // string != end
    {
        if( *wild == '*' )
        {
            ++wild;
            if( wild == wildEnd )
                return true;

            mp = wild;
            cp = str+1;
        }
        else if( (*wild == *str) || (*wild == '?') )
        {
            ++wild;
            ++str;
        }
        else
        {
            wild = mp;
            str = cp;
            if( *str == '/' )
                return false;

            ++cp;
        }
    }
    while( wild != wildEnd && *wild == '*' )
    {
        ++wild;
    }
    return ( wild==wildEnd );
}

} // namespace ambrosia
