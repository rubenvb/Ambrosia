TEMPLATE = subdirs

SUBDIRS += Library \
           Application

Application.depends = Library

OTHER_FILES += Ambrosia.nectar.txt \
               "Environment detection.txt"
