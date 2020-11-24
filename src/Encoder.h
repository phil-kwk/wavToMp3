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
 * File:   Encoder.h
 * Author: Philipp Kaaden
 *
 * Created on 13. November 2020, 14:34
 */

#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <exception>
#include <string>
#include <cstdint>

#include "Formats.h"

namespace encoder {

    enum CodeExceptionEncoder {
        WAVE_FORMAT_NOT_SUPPORTED = 10,
        SAMPLE_SIZE_RL_NOTEQUAL = 20
    };

    class ExceptionEncoder : std::exception {
    public:
        ExceptionEncoder(const CodeExceptionEncoder code);
        const CodeExceptionEncoder code();
    private:
        const CodeExceptionEncoder errcode;
    };

    /**
     * encode wavePcm to mp3Frames in current thread
     * @param fmt format Header of WAVE File
     * @param samples wavePcm samples in ChannelContainer
     * @return mp3Frames
     * @throw ExceptionEncoder
     */
    template<typename SampleTyp>
    const std::vector<uint8_t> wavToMp3Seriell(
            const WaveFmtHeader& fmt,
            const ChannelContainer<SampleTyp> samples);

    /**
     * encode wavePcm to mp3Frames with mulitple threads
     * @param fmt format Header of WAVE File
     * @param samples wavePcm samples in ChannelContainer
     * @param numThreads for use then encoding (0 try to use default thread number)
     * @return mp3Frames
     * @throw ExceptionEncoder
     */
    template<typename SampleTyp>
    const std::vector<uint8_t> wavToMp3Concurrently(
            const WaveFmtHeader& fmt,
            const ChannelContainer<SampleTyp> samples,
            unsigned int numThreads = 0);

}//end namespace
#endif /* ENCODER_H */

