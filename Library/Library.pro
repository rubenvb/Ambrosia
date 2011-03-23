TEMPLATE = lib
CONFIG += static

CONFIG -= qt
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

*g++*:QMAKE_CXXFLAGS += -std=c++0x -pedantic -Wextra \
                        -Weffc++ -Wmissing-include-dirs -Wstrict-aliasing

CONFIG( debug, debug|release ) {
    LIBSUFFIX = d
    DEFINES += AMBROSIA_DEBUG
} else {
    LIBSUFFIX =
}
TARGET = Ambrosia$${LIBSUFFIX}

#DEFINES += AMBROSIA_DLL AMRBOSIA_BUILD_DLL

INCLUDEPATH += . Include Include/Ambrosia Include/Parser
DEPENDPATH += . Include Source

HEADERS += \
    Include/Ambrosia/algorithm.h \
    Include/Ambrosia/debug.h \
    Include/Ambrosia/enums.h \
    Include/Ambrosia/global.h \
    Include/Ambrosia/platform.h \
    Include/Ambrosia/typedefs.h \
    Include/Ambrosia/state.h \
    Include/project.h \
    Include/Ambrosia/enum_maps.h \
    Include/Ambrosia/build_config.h \
    Include/parser.h \
    Include/Ambrosia/status.h \
    Include/nectar_loader.h \
    Include/Ambrosia/nectar.h \
    Include/Ambrosia/target.h

SOURCES += \
    Source/Ambrosia/Platform/common.cpp \
    Source/Ambrosia/Platform/linux.cpp \
    Source/Ambrosia/Platform/windows.cpp \
    Source/Ambrosia/algorithm.cpp \
    Source/Ambrosia/state.cpp \
    Source/project.cpp \
    Source/Ambrosia/enum_maps.cpp \
    Source/nectar_loader.cpp \
    Source/Ambrosia/build_config.cpp \
    Source/parser.cpp \
    Source/Ambrosia/status.cpp \
    Source/Ambrosia/nectar.cpp \
    Source/Ambrosia/target.cpp

*win32*:SOURCES +=
*linux*:SOURCES +=
