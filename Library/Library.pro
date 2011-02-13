TEMPLATE = lib
CONFIG += static

CONFIG -= qt
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

*g++*:QMAKE_CXXFLAGS += -std=c++0x -pedantic -Wextra \
                        -Weffc++ -Wmissing-include-dirs -Wstrict-aliasing

CONFIG( debug, debug|release ) {
    LIBSUFFIX = d
} else {
    LIBSUFFIX =
}
TARGET = Ambrosia$${LIBSUFFIX}

INCLUDEPATH += . Include Include/Ambrosia Include/Parser
DEPENDPATH += . Include Source

HEADERS += \
    Include/FileStore.h \
    Include/Reader.h \
    Include/Ambrosia/Algorithm.h \
    Include/Ambrosia/Enums.h \
    Include/Ambrosia/Platform.h \
    Include/Ambrosia/Typedefs.h \
    Include/Ambrosia/Debug.h \
    Include/Target.h \
    Include/Ambrosia/Error.h \
    Include/Ambrosia/State.h \
    Include/Ambrosia/Global.h

SOURCES += \
    Source/FileStore.cpp \
    Source/Reader.cpp \
    Source/Ambrosia/Algorithm.cpp \
    Source/Target.cpp \
    Source/Ambrosia/Error.cpp \
    Source/Ambrosia/State.cpp

*win32*:SOURCES += Source/Ambrosia/Platform_Windows.cpp
*linux*:SOURCES += Source/Ambrosia/Platform_Linux.cpp
