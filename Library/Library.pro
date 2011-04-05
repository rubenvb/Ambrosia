TEMPLATE = lib
CONFIG += static

CONFIG -= qt
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

*g++*:QMAKE_CXXFLAGS += -std=c++0x -pedantic -Wextra \
                        -Weffc++ -Wmissing-include-dirs -Wstrict-aliasing

CONFIG( debug, debug|release ) {
    LIBSUFFIX = d
    DEFINES += AMBROSIA_DEBUG
    QMAKE_CXXFLAGS += -g3
    QMAKE_LFLAGS -= -s
} else {
    LIBSUFFIX =
}
TARGET = Ambrosia$${LIBSUFFIX}

#DEFINES += AMBROSIA_DLL AMRBOSIA_BUILD_DLL

INCLUDEPATH += . Include Include/Ambrosia
DEPENDPATH += . Include Source

HEADERS += \
    Include/Ambrosia/algorithm.h \
    Include/Ambrosia/debug.h \
    Include/Ambrosia/enums.h \
    Include/Ambrosia/global.h \
    Include/Ambrosia/platform.h \
    Include/Ambrosia/typedefs.h \
    Include/Ambrosia/state.h \
    Include/Ambrosia/enum_maps.h \
    Include/Ambrosia/build_config.h \
    Include/Ambrosia/status.h \
    Include/Ambrosia/nectar.h \
    Include/Ambrosia/target.h \
    Include/Ambrosia/Parser/parser_state.h \
    Include/Ambrosia/nectar_loader.h

SOURCES += \
    Source/Ambrosia/Platform/common.cpp \
    Source/Ambrosia/algorithm.cpp \
    Source/Ambrosia/state.cpp \
    Source/Ambrosia/enum_maps.cpp \
    Source/Ambrosia/build_config.cpp \
    Source/Ambrosia/status.cpp \
    Source/Ambrosia/nectar.cpp \
    Source/Ambrosia/target.cpp \
    Source/Ambrosia/Parser/parser_state.cpp \
    Source/Ambrosia/nectar_loader.cpp

*win32*:SOURCES += Source/Ambrosia/Platform/windows.cpp
*linux*:SOURCES += Source/Ambrosia/Platform/linux.cpp
