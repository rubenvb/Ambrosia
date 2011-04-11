TEMPLATE = subdirs

SUBDIRS += libAmbrosia \
           Ambrosia

Application.depends = libAmbrosia

OTHER_FILES += Ambrosia.nectar.txt \
               "Environment detection.txt" \
               Cleanup.txt \
               Codestyle.txt \
               Ideas.txt \
               "Planned unit tests.txt"
