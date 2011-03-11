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
    Include/Ambrosia/error.h \
    Include/Ambrosia/global.h \
    Include/Ambrosia/platform.h \
    Include/Ambrosia/typedefs.h \
    Include/Ambrosia/state.h \
    Include/file_store.h \
    Include/project.h \
    Include/target.h \
    Include/Ambrosia/enum_maps.h \
    Include/nectar_loader.h \
    Include/Ambrosia/build_config.h

SOURCES += \
    Source/Ambrosia/Platform/common.cpp \
    Source/Ambrosia/Platform/linux.cpp \
    Source/Ambrosia/Platform/windows.cpp \
    Source/Ambrosia/algorithm.cpp \
    Source/Ambrosia/error.cpp \
    Source/Ambrosia/state.cpp \
    Source/file_store.cpp \
    Source/project.cpp \
    Source/target.cpp \
    Source/Ambrosia/enum_maps.cpp \
    Source/nectar_loader.cpp \
    Source/Ambrosia/build_config.cpp

*win32*:SOURCES +=
*linux*:SOURCES +=
