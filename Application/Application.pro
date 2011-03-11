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
    Source/begin.cpp \
    Source/main.cpp \
    Source/end_state.cpp \
    Source/help_and_version_output.cpp \
    Source/output.cpp \
    Source/reader.cpp \
    Source/builder.cpp

HEADERS += \
    Include/output.h \
    Include/begin.h \
    Include/global.h \
    Include/end_state.h \
    Include/help_and_version_output.h \
    Include/reader.h \
    Include/builder.h
