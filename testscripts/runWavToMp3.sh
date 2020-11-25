#!/bin/sh
# 
# File:   runWavToMp3.sh
# Author: Philipp Kaaden
#
# Created on 23.11.2020, 10:04:13
#
execPath=../build/src/wavToMp3

#delete all files but shell scripts
find . -type f \( -name '*mp3' \) -delete

./$execPath

./$execPath .

./$execPath //

./$execPath ///

./$execPath . ../../../


#delete artefacts afterwards
find . -type f -not \( -name '*sh' \) -delete