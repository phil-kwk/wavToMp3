#!/bin/sh
# 
# File:   runValgrindCheck.sh
# Author: Philipp Kaaden
#
# Created on 23.11.2020, 10:03:50
#

execPath=/../build/src/wavToMp3

#valgrind --tool=memcheck --leak-check=yes .$execPath

#valgrind --tool=memcheck --leak-check=yes .$execPath ../testfiles/

#delete old massif.out.*
echo y | find -name "massif.out.*" -exec rm -i {} \;

valgrind --tool=massif .$execPath
ms_print massif.out.*