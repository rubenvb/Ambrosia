TEMPLATE = lib
CONFIG += static

CONFIG -= qt
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

*g++*:QMAKE_CXXFLAGS += -std=c++0x -pedantic -Wextra -Wall -Wconversion \
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
    Include/Ambrosia/algorithm.h \
    Include/Ambrosia/node.h \
    Include/Ambrosia/Parser/parser.h \
    Include/Ambrosia/Parser/nectar_loader.h

SOURCES += \
    Source/Ambrosia/Platform/common.cpp \
    Source/Ambrosia/state.cpp \
    Source/Ambrosia/enum_maps.cpp \
    Source/Ambrosia/build_config.cpp \
    Source/Ambrosia/status.cpp \
    Source/Ambrosia/nectar.cpp \
    Source/Ambrosia/target.cpp \
    Source/Ambrosia/algorithm.cpp \
    Source/Ambrosia/debug.cpp \
    Source/Ambrosia/node.cpp \
    Source/Ambrosia/Parser/parser.cpp \
    Source/Ambrosia/Parser/nectar_loader.cpp

*win32*:SOURCES += Source/Ambrosia/Platform/windows.cpp
*linux*:SOURCES += Source/Ambrosia/Platform/linux.cpp

OTHER_FILES += libAmbrosia.nectar.txt
