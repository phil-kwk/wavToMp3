/*
 * Copyright (C) 2020 Philipp Kaaden.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

/* 
 * File:   FileEncodingTask.cpp
 * Author: Philipp Kaaden
 * 
 * Created on 22. November 2020, 17:35
 */

#include "FileEncodingTask.h"

#include "Parser.h"
#include "SampleConverter.h"
#include "Encoder.h"

#include <thread>
#include <iostream>
#include <cmath>

namespace encoder {

    namespace {

        const std::size_t PCM_SAMPLES_PER_MP3_FRAME = 1152;


        const std::size_t BYTES_PEEK_INTO_FILE = 1000;

        const std::string WAVE_FILE_EXTENSION{".wav"};
        const std::string MP3_FILE_EXTENSION{".mp3"};

        const std::string getOutputFileName(const std::string& inName) {
            std::string outName{ inName};
            auto pos = outName.find_last_of(WAVE_FILE_EXTENSION);
            outName.replace(pos - WAVE_FILE_EXTENSION.size() + 1, WAVE_FILE_EXTENSION.size(), MP3_FILE_EXTENSION);
            return outName;
        }

        const WAVE_Format getWaveFormat(ReadFile& read) {
            auto buffer = read.peek(BYTES_PEEK_INTO_FILE);
            return get_WaveFormat(buffer);
        }

        const std::size_t getSampleSize(
                const WAVE_Format& wave,
                const std::size_t bufferSize) {
            return bufferSize / wave.fmt.blockAlignment;
        }

        const bool using_uint8Samples(const WaveFmtHeader& fmt) {
            if ((fmt.blockAlignment / fmt.channels) == 1) {
                return true;
            }
            return false;
        }

        const bool using_int16Samples(const WaveFmtHeader& fmt) {
            if ((fmt.blockAlignment / fmt.channels) == 2) {
                return true;
            }
            return false;
        }

        const bool using_int32Samples(const WaveFmtHeader& fmt) {
            if ((fmt.blockAlignment / fmt.channels) == 4) {
                return true;
            }
            return false;
        }

        const bool checkSampleFormat(const WaveFmtHeader fmt) {
            if (using_int32Samples(fmt)) {
                return true;
            } else if (using_int16Samples(fmt)) {
                return true;
            } else if (using_uint8Samples(fmt)) {
                return true;
            }
            return false;
        }
    }

    void startFileEncoding(const std::string filename,
            const std::size_t max_memory_consumtion,
            const std::size_t numberThreads) {

        ReadFile read(filename);
        auto buffer = read.peek(BYTES_PEEK_INTO_FILE);
        WAVE_Format wave;
        try {
            wave = get_WaveFormat(buffer);
        } catch (ParseWAVFileError exc) {
            std::cout << filename << " ParseWAVFileError " << exc.code() << "\n";
            return;
        }

        if (!checkSampleFormat(wave.fmt)) {
            return;
        }

        std::cout << getOutputFileName(filename);
        read.next(getDataStartPosition(buffer));

        const std::size_t BYTES_READ_FROM_FILE = floor(max_memory_consumtion / PCM_SAMPLES_PER_MP3_FRAME) * PCM_SAMPLES_PER_MP3_FRAME;

        WriteFile write(getOutputFileName(filename));
        std::vector<uint8_t> mp3Frames;
        do {
            buffer = read.next(BYTES_READ_FROM_FILE);
            if (using_int16Samples(wave.fmt)) {
                auto pcm = get_pcm_Samples<int16_t>(buffer, getSampleSize(wave, buffer.size()), wave.fmt.channels);
                mp3Frames = wavToMp3Concurrently(wave.fmt, pcm, numberThreads);
            } else if (using_int32Samples(wave.fmt)) {
                auto pcm = get_pcm_Samples<int32_t>(buffer, getSampleSize(wave, buffer.size()), wave.fmt.channels);
                mp3Frames = wavToMp3Concurrently(wave.fmt, pcm, numberThreads);
            } else if (using_uint8Samples(wave.fmt)) {
                auto pcm = get_pcm_Samples<uint8_t>(buffer, getSampleSize(wave, buffer.size()), wave.fmt.channels);
                mp3Frames = wavToMp3Concurrently(wave.fmt, pcm, numberThreads);
            } else {
                return;
            }
            write.write(mp3Frames);
        } while (buffer.size() == BYTES_READ_FROM_FILE);
        std::cout << " done" << "\n";
    }

}