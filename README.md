# wavToMp3
Demo Project for Multithreaded encoding of a WAVE to MP3 written in c++11 using LAME Encoder for mp3 Frame encoding.

* see INSTALL file for information on how to build the Project

The wavToMp3 encoder is currently capable of encoding RAW PCM mono, stereo signals with a sample size of (uint8, int16, int32) more Sample Sizes can be easily added.

The wavToMp3 encoder takes Directory Paths as arguments and try's to encode all wav Files in the directory to mp3

Parallel Encoding Workflow:
1. part of the wav File is loaded into wavBuffer
2. wavBuffer is sliced into chunks that are multiples of 1152 Samples 
3. For each Chunk a cpp11 async tasks is started for encoding of Chunk to MP3 Frames
4. Synchronisation of the async tasks is done and MP3 Frames are written to mp3 File
5. Restart at 1 and load next part of File

