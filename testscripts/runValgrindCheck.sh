#!/bin/sh
# 
# File:   runValgrindCheck.sh
# Author: Philipp Kaaden
#
# Created on 23.11.2020, 10:03:50
#

execPath=../build/src/wavToMp3

#delete all files but shellscripts
find . -type f -not \( -name '*sh' \) -delete

#valgrind --tool=memcheck --leak-check=yes .$execPath

#valgrind --tool=memcheck --leak-check=yes .$execPath ../testfiles/

valgrind --tool=massif ./$execPath ../testsound
ms_print massif.out.*


#clean up artefacts
find . -type f -not \( -name '*sh' \) -delete