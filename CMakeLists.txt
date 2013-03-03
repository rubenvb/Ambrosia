##
# Project Ambrosia: main CMake project file
#
# Written in 2012 by Ruben Van Boxem <vanboxem.ruben@gmail.com>
#
# To the extent possible under law, the author(s) have dedicated all copyright and related
# and neighboring rights to this software to the public domain worldwide. This software is
# distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
#

cmake_minimum_required(VERSION 2.8)
project(Ambrosia)

if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
set(CMAKE_CXX_FLAGS "-std=c++0x -pedantic-errors -Wextra -Wconversion -Wuninitialized -Winit-self -Wmissing-include-dirs -Wstrict-aliasing -Werror ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_LINK_FLAGS "-static ${CMAKE_CXX_LINK_FLAGS}")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
set(CMAKE_CXX_FLAGS "-std=c++0x -pedantic-errors -Wextra -Wconversion -Wuninitialized -Winit-self -Wmissing-include-dirs -Wstrict-aliasing -Werror ${CMAKE_CXX_FLAGS}")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
add_definitions(-DAMBROSIA_DEBUG)
endif()

file(GLOB libAmbrosia_sources libAmbrosia/Source/*.cpp libAmbrosia/Source/Error/*.cpp libAmbrosia/Source/Target/*.cpp libAmbrosia/Source/Platform/common.cpp)
if(WIN32)
  set(libAmbrosia_sources ${libAmbrosia_sources} libAmbrosia/Source/Platform/windows.cpp)
elseif(UNIX)
  set(libAmbrosia_sources ${libAmbrosia_sources} libAmbrosia/Source/Platform/unix.cpp)
endif()
file(GLOB libAmbrosia_headers libAmbrosia/Include/Ambrosia/*.h libAmbrosia/Include/Ambrosia/Error/*.h libAmbrosia/Include/Ambrosia/Target/*.h )

include_directories("${Ambrosia_SOURCE_DIR}/libAmbrosia/Include")

add_library(libAmbrosia ${libAmbrosia_sources} ${libAmbrosia_headers})
set_target_properties(libAmbrosia PROPERTIES OUTPUT_NAME "Ambrosia")

file(GLOB Ambrosia_sources Ambrosia/Source/*.cpp)
file(GLOB Ambrosia_headers Ambrosia/Include/*.h)

include_directories("${Ambrosia_SOURCE_DIR}/Ambrosia/Include")
add_executable(Ambrosia ${Ambrosia_sources} ${Ambrosia_headers})
target_link_libraries(Ambrosia libAmbrosia)
if(WIN32)
  target_link_libraries(Ambrosia shlwapi)
endif()
