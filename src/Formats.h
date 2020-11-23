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
 * File:   WAVERepresentation.h
 * Author: Philipp Kaaden
 *
 * Created on 13. November 2020, 15:32
 */

#ifndef WAVEREPRESENTATION_H
#define WAVEREPRESENTATION_H


#include <cstdint>
#include <vector>

namespace encoder {

    template<typename SampleType>
    using Channel = std::vector<SampleType>; //{1,2,3,4,...}

    /**
     * { {1,2,3,4, ..},
     *   {5,6,7,8, ..} }
     */
    template<typename SampleType>
    using ChannelContainer = std::vector< Channel<SampleType> >;

    /**
     *{{ {1,2},
     *   {5,6} },
     * { {3,4},
     *   {7,8} },
     * { {..},
     *   {..} }}
     */
    template<typename SampleType>
    using SlicedChannelContainer = std::vector< ChannelContainer<SampleType> >;

    struct RiffHeader {
        uint8_t groupId[4]; //"RIFF" for Riff File
        uint32_t fileLength; //Length in Bytes with offset 8
        uint8_t riftType[4]; //"WAVE" in WAVE Files
    }; //12Byte

    struct WaveFmtHeader {
        char chunkId[4]; //"fmt "
        uint32_t chunkSize; //length of following format Header in Bytes
        uint16_t formatTag; //1 uncompressed PCM
        uint16_t channels; //number of channels
        uint32_t SamplesPerSec; //SampleRate in Hz
        uint32_t bytesPerSecond; //for RAM allocation
        uint16_t blockAlignment; //Sample Frame Size
        uint16_t bitsPerSample; // Bits Per sample of one channel
    }; //24Byte

    struct WaveDataHeader {
        char chunkId[4]; //"data"
        uint32_t dataSize; //length of following Data in Bytes
    }; //8Byte + Sampledata

    struct WAVE_Format {
        RiffHeader riff;
        WaveFmtHeader fmt;
        WaveDataHeader data;
    };



}//end namespace

#endif /* WAVEREPRESENTATION_H */

