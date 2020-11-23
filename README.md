# wavToMp3
Demo Project for Multithreaded encoding of WAV to MP3 written in c++11 using LAME Encoder for mp3 Frame encoding.

* see INSTALL file for information on how to build the Project

The wavToMp3 encoder is currently capable of encoding RAW PCM mono, stereo signals with a sample size of (uint8, int16, int32) more Sample Sizes can be easily added.

The wavToMp3 encoder takes Directory Paths as arguments and try's to encode all wav Files in the directory to mp3

## Parallel Encoding Workflow:
1. part of the wav File is loaded into wavBuffer
2. wavBuffer is sliced into chunks that are multiples of 1152 Samples 
3. For each Chunk a cpp11 async tasks is started for encoding of Chunk to MP3 Frames
4. Synchronisation of the async tasks is done and MP3 Frames are written to mp3 File
5. Return to 1 and load next part of File

## Reasons for this Workflow approach:
* wav Files can possibly be very big (up to 4GB with uint32 File Size Field and with uint64 size field 2^64 Byte Big)
* loading of Samples into Buffer and preparation of encoding mp3 buffer takes up great amount of heap allocated memory space
* encoding by using multiple tasks speeds up time to encode massively

## Problems with this Workflow approach
* since mp3 Frames are attached to each other without knowing each other a audio mismatch can be observed at the attachment point

## TODOs for this Project
* the Error Handling needs improvement
* Refactoring for better readability is needed
* some parts are still missing good unit testing


## Simple Call Diagramm

![Call Diagramm](/doc/Call_Diagramm_wavToMp3.png)