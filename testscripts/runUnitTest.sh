#!/bin/sh
# 
# File:   runUnitTest.sh
# Author: Philipp Kaaden
#
# Created on 23.11.2020, 10:03:30
#

find . -type f -not \( -name '*sh' \) -delete

execPath=/../build/test/wavToMp3UnitTests

.$execPath

#delete artefacts afterwards
find . -type f -not \( -name '*sh' \) -delete