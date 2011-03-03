/**
  * build_config.cpp
  * Class implementation.
  *
  * Author: Ruben Van Boxem
  *
  **/

// Class include
#include "build_config.h"

// libAmbrosia includes
#include "Ambrosia/debug.h"
#include "Ambrosia/error.h"
#include "Ambrosia/platform.h"
/* "Ambrosia/typedefs.h" */

// C++ includes
/* <map> */
    using std::map;
/* <string> */
    using std::string;

namespace ambrosia
{
    // static member initialization
    const map<string, architecture> build_config::s_architecture_map = { {"x86", architecture::x86},
                                                                         {"amd64", architecture::amd64} };
    const map<string, os> build_config::s_os_map = { {"windows", os::Windows},
                                                     {"linux", os::Linux},
                                                     {"mac", os::MacOSX} };
    const map<string, toolchain> s_toolchain_map = { {"gcc", toolchain::GNU},
                                                     {"msvc", toolchain::Microsoft},
                                                     {"clang", toolchain::LLVM},
                                                     {"intel", toolchain::Intel} };
    build_config::build_config()
    :   m_target_os( build_os ),
        m_target_architecture( build_architecture ),
        m_gnu_prefix( string() ),
        m_user_options( string_map() ),
        m_target_config( string_map() )
    {   }

    void build_config::set_ambrosia_cross( const std::string &cross )
    {
        debug() << "build_config::Checking and setting cross-compilation options through Ambrosia specification.\n";
        // locate bitness
        const size_t bitness_index = std::min( cross.find("32"), cross.find("64") );
        // locate toolchain
        const size_t toolchain_index = cross.find( "-" );

        if( bitness_index == string::npos || toolchain_index == string::npos )
        {
            error::emit_error( "Ambrosia cross-compile specification should be of the form \'OSBitness-Toolchain\'.\n" );
            return;
        }
        // split up the string
        const string os( cross.substr(0, bitness_index) );
        const string bitness( cross.substr(bitness_index, 2) );
        const string toolchain( cross.substr(bitness_index+3, string::npos ) );
        debug() << "build_config::os = " << os << ", bitness: " << bitness << ", toolchain: " << toolchain << ".\n";


    }
    void build_config::set_gnu_prefix( const std::string &prefix )
    {
        // TODO: detect and set common GNU/GCC target triplets
    }
    void add_target_config( const std::string &target, const std::string &config )
    {

    }
} // namespace ambrosia
