TEMPLATE = app
CONFIG += console warn_on

CONFIG -= qt
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

*g++*:QMAKE_CXXFLAGS += -std=c++0x -pedantic -Wextra \
                        -Weffc++ -Wmissing-include-dirs -Wstrict-aliasing

# Ambrosia Library
INCLUDEPATH += ../Library/Include
CONFIG( debug, debug|release ) {
    LIBSUFFIX = d
    LIBS += -L../Library/debug
    DEFINES += AMBROSIA_DEBUG
} else {
    LIBSUFFIX =
    LIBS += -L../Library/release
}
LIBS += -lAmbrosia$${LIBSUFFIX}
#DEFINES += AMBROSIA_DLL

# Application includes
INCLUDEPATH += . Include
DEPENDPATH += . Include Source

SOURCES += \
    Source/Main.cpp \
    Source/Begin.cpp \
    Source/End.cpp

HEADERS += \
    Include/Begin.h \
    Include/Output.h \
    Include/End.h \
    Include/Global.h
