TEMPLATE = app
CONFIG += console warn_on

CONFIG -= qt
DEFINES -= UNICODE QT_LARGEFILE_SUPPORT

*g++*:QMAKE_CXXFLAGS += -std=c++0x -pedantic-errors -Wextra -Wconversion \
                        -Weffc++ -Wmissing-include-dirs -Wstrict-aliasing -Werror
# Ambrosia Library
INCLUDEPATH += ../libAmbrosia/Include
CONFIG( debug, debug|release ) {
    LIBSUFFIX = d
    win32:LIBS += -L../libAmbrosia/debug
    win32:PRE_TARGETDEPS += ../libAmbrosia/debug/libAmbrosiad.a
    DEFINES += AMBROSIA_DEBUG
} else {
    LIBSUFFIX =
    win32:LIBS += -L../libAmbrosia/release
    win32:PRE_TARGETDEPS += ../libAmbrosia/release/libAmbrosia.a
}
unix:LIBS += -L ../libAmbrosia
unix:PRE_TARGETDEPS += ../libAmbrosia/libAmbrosia$${LIBSUFFIX}.a
LIBS += -lAmbrosia$${LIBSUFFIX}
win32:LIBS += -lshlwapi
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
#DEFINES += AMBROSIA_DLL

# Application includes
INCLUDEPATH += . Include
DEPENDPATH += . Include Source

SOURCES += \
    Source/main.cpp \
    Source/help_and_version_output.cpp \
    Source/output.cpp \
    Source/commandline.cpp

HEADERS += \
    Include/output.h \
    Include/global.h \
    Include/help_and_version_output.h \
    Include/commandline.h

OTHER_FILES += Ambrosia.nectar.txt




















