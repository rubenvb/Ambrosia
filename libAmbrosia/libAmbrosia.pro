TEMPLATE = lib
CONFIG += static

CONFIG -= qt
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

*g++*:QMAKE_CXXFLAGS += -std=c++0x -pedantic-errors -Wextra -Wall -Wconversion \
                        -Weffc++ -Wmissing-include-dirs -Wstrict-aliasing -Werror
CONFIG( debug, debug|release ) {
    LIBSUFFIX = d
    DEFINES += AMBROSIA_DEBUG
    *g++*:QMAKE_CXXFLAGS += -g3
} else {
    LIBSUFFIX =
}
TARGET = Ambrosia$${LIBSUFFIX}

#DEFINES += AMBROSIA_DLL AMRBOSIA_BUILD_DLL

INCLUDEPATH += . Include
DEPENDPATH += . Include Source

HEADERS += \
    Include/Ambrosia/debug.h \
    Include/Ambrosia/enums.h \
    Include/Ambrosia/global.h \
    Include/Ambrosia/platform.h \
    Include/Ambrosia/typedefs.h \
    Include/Ambrosia/enum_maps.h \
    Include/Ambrosia/status.h \
    Include/Ambrosia/nectar.h \
    Include/Ambrosia/target.h \
    Include/Ambrosia/algorithm.h \
    Include/Ambrosia/node.h \
    Include/Ambrosia/nectar_loader.h \
    Include/Ambrosia/Configuration/ambrosia_config.h \
    Include/Ambrosia/Configuration/build_config.h \
    Include/Ambrosia/Configuration/config_base.h \
    Include/Ambrosia/project.h \
    Include/Ambrosia/Error/syntax_error.h \
    Include/Ambrosia/Error/error.h \
    Include/Ambrosia/Error/nectar_error.h \
    Include/Ambrosia/file_cache.h \
    Include/Ambrosia/Error/soft_error.h \
    Include/Ambrosia/Error/commandline_error.h \
    Include/Ambrosia/Error/internal_error.h \
    Include/Ambrosia/Generators/cgenerator.h \
    Include/Ambrosia/Generators/generator.h \
    Include/Ambrosia/Generators/generator_maps.h

SOURCES += \
    Source/Configuration/ambrosia_config.cpp \
    Source/Configuration/build_config.cpp \
    Source/Configuration/config_base.cpp \
    Source/Error/commandline_error.cpp \
    Source/Error/error.cpp \
    Source/Error/internal_error.cpp \
    Source/Error/nectar_error.cpp \
    Source/Error/soft_error.cpp \
    Source/Error/syntax_error.cpp \
    Source/Generators/cgenerator.cpp \
    Source/Error/generator.cpp \
    Source/Error/generator_maps.cpp \
    Source/Platform/unix.cpp \
    Source/Platform/common.cpp \
    Source/Platform/windows.cpp \
    Source/algorithm.cpp \
    Source/debug.cpp \
    Source/enum_maps.cpp \
    Source/file_cache.cpp \
    Source/nectar.cpp \
    Source/nectar_loader.cpp \
    Source/node.cpp \
    Source/project.cpp \
    Source/status.cpp \
    Source/target.cpp

*win32*:SOURCES +=
*linux*:SOURCES +=
*mac*:SOURCES +=

OTHER_FILES += libAmbrosia.nectar.txt


























