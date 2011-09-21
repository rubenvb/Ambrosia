TEMPLATE = subdirs

SUBDIRS += libAmbrosia \
           Ambrosia

CONFIG += debug_and_release

Application.depends = libAmbrosia

OTHER_FILES += Ambrosia.nectar.txt \
               "Environment detection.txt" \
               Cleanup.txt \
               Codestyle.txt \
               Ideas.txt \
               "Planned unit tests.txt"
