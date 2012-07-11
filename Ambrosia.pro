##
# Project Ambrosia: main project file
#
# Written in 2012 by Ruben Van Boxem <vanboxem.ruben@gmail.com>
#
# To the extent possible under law, the author(s) have dedicated all copyright and related
# and neighboring rights to this software to the public domain worldwide. This software is
# distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
#

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
               "Planned unit tests.txt" \
               Copying.txt
