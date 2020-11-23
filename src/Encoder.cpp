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
 * File:   Encoder.cpp
 * Author: Philipp Kaaden
 * 
 * Created on 13. November 2020, 14:34
 */

#include "Encoder.h"
#include "LameEncoder.h"

#include "Formats.h"
#include "Converter.h"

#include <stdint.h>

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <future>

namespace encoder {

    ExceptionEncoder::ExceptionEncoder(const CodeExceptionEncoder code) :
    errcode(code) {

    }

    const CodeExceptionEncoder ExceptionEncoder::msg() {
        return errcode;
    }

    template<typename SampleType>
    const std::vector<uint8_t> wavToMp3Seriell(
            const WaveFmtHeader& fmt,
            const ChannelContainer<SampleType> samples) {

        LameEncoder<SampleType> encoder;
        encoder.setConfig(fmt, good);

        auto mp3Buffer = encoder.encodePCMSamples(samples);
        auto flushBuffer = encoder.encode_flush(false); //flush is called at end of encoding to output last mp3frame and write idv3 Header

        mp3Buffer.insert(mp3Buffer.end(), flushBuffer.begin(), flushBuffer.end());
        return mp3Buffer;
    }
    template const std::vector<uint8_t> wavToMp3Seriell(const WaveFmtHeader& fmt, const ChannelContainer<uint8_t> samples);
    template const std::vector<uint8_t> wavToMp3Seriell(const WaveFmtHeader& fmt, const ChannelContainer<int16_t> samples);
    template const std::vector<uint8_t> wavToMp3Seriell(const WaveFmtHeader& fmt, const ChannelContainer<int32_t> samples);

    template<typename SampleType>
    const std::vector<uint8_t> encodingTask(
            const WaveFmtHeader& fmt,
            const ChannelContainer<SampleType> & sampleBuffer) {
        std::vector<uint8_t> buffer;
        try {
            LameEncoder<SampleType> encoder;
            encoder.setConfig(fmt, good);
            buffer = encoder.encodePCMSamples(sampleBuffer);
            auto flush = encoder.encode_flush(true);
            buffer.insert(buffer.end(), flush.begin(), flush.end());
        } catch (ExceptionLameEncoding exc) {
            std::cout << "ExceptionLameEncoding " << exc.msg();
            return {};
        }
        return buffer;
    };
    template const std::vector<uint8_t> encodingTask(const WaveFmtHeader& fmt, const ChannelContainer<uint8_t> & sampleBuffer);
    template const std::vector<uint8_t> encodingTask(const WaveFmtHeader& fmt, const ChannelContainer<int16_t> & sampleBuffer);
    template const std::vector<uint8_t> encodingTask(const WaveFmtHeader& fmt, const ChannelContainer<int32_t> & sampleBuffer);

    template<typename SampleType>
    const std::vector<uint8_t> wavToMp3Concurrently(
            const WaveFmtHeader& fmt,
            const ChannelContainer<SampleType> samples,
            unsigned int numThreads) {

        if (numThreads == 0) {
            numThreads = std::thread::hardware_concurrency();
        }
        auto slices = slice_PCMSamples<SampleType>(samples, numThreads);

        std::vector< std::future< const std::vector<uint8_t> > > tasks;
        for (auto s : slices) {
            auto future = std::async(encodingTask<SampleType>, fmt, s);
            tasks.push_back(std::move(future));
        }

        std::vector<uint8_t> mp3Buffer;
        for (std::size_t n = 0; n < tasks.size(); n++) {//range based loop cannot be used here problem with Typ inferenz Some Compilers may have problems with the use of async
            auto b = tasks.at(n).get();
            mp3Buffer.insert(mp3Buffer.end(), b.begin(), b.end());
        }
        return mp3Buffer;
    }
    template const std::vector<uint8_t> wavToMp3Concurrently(const WaveFmtHeader& fmt, const ChannelContainer<uint8_t> samples, unsigned int numThreads);
    template const std::vector<uint8_t> wavToMp3Concurrently(const WaveFmtHeader& fmt, const ChannelContainer<int16_t> samples, unsigned int numThreads);
    template const std::vector<uint8_t> wavToMp3Concurrently(const WaveFmtHeader& fmt, const ChannelContainer<int32_t> samples, unsigned int numThreads);

}//end namespace