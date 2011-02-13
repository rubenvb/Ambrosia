TEMPLATE = subdirs

SUBDIRS += Library \
           Application

Application.depends = Library
