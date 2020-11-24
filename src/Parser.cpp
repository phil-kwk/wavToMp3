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
 * File:   WAVFileBuffer.cpp
 * Author: Philipp Kaaden
 * 
 * Created on 12. November 2020, 16:30
 */

#include "Parser.h"
#include "SampleConverter.h"

#include <vector>
#include <iostream>
#include <algorithm>
#include <string.h>


namespace encoder {

    namespace {
        const uint32_t RIFF_FILE_HEADER_LENGTH = 12;
        const uint32_t RIFF_WAV_FORMAT_HEADER_LENGTH = 24;
        const uint32_t RIFF_WAV_DATA_HEADER_LENGTH = 8;


        const uint32_t MIN_FORMAT_HEADER_LENGTH = 16; //Chunk size: 18 or 40 only possible if not WAVE_FORMAT_PCM

        const char* RIFF_ID = "RIFF";
        const char* WAVE_ID = "WAVE";

        const char* FORMAT_ID = "fmt ";
        const std::vector<uint8_t> FORMAT_CHUNKID{ 'f', 'm', 't', ' '};

        const char* DATA_ID = "data";
        const std::vector<uint8_t> DATA_CHUNKID{ 'd', 'a', 't', 'a'};

        const uint32_t WAVE_FORMAT_PCM = 0x0001;
        const uint32_t WAVE_FORMAT_IEEE_FLOAT = 0x0003;
        const uint32_t WAVE_FORMAT_ALAW = 0x0006;
        const uint32_t WAVE_FORMAT_MULAW = 0x0007;
        const uint32_t WAVE_FORMAT_EXTENSIBLE = 0xFFFE;

        template<typename SampleTyp>
        const SampleTyp get_Value(
                const std::vector<uint8_t>& buffer,
                const std::size_t position);

        template<>
        const uint16_t get_Value<uint16_t>(
                const std::vector<uint8_t>& buffer,
                const std::size_t position) {
            uint16_t val = uint16_t(buffer.at(position + 1) << 8) | uint16_t(buffer.at(position));
            return val;
        }

        template<>
        const uint32_t get_Value<uint32_t>(
                const std::vector<uint8_t>& buffer,
                const std::size_t position) {
            uint32_t val = uint32_t(buffer.at(position + 3) << 24) | uint32_t(buffer.at(position + 2) << 16) |
                    uint32_t(buffer.at(position + 1) << 8) | uint32_t(buffer.at(position));
            return val;
        }
    }

    ParseWAVFileError::ParseWAVFileError(const ParseWAVFileErrorCodes msg) :
    errcode(msg) {
    }

    const ParseWAVFileErrorCodes ParseWAVFileError::code() {
        return errcode;
    }

    const WAVE_Format
    get_WaveFormat(const std::vector<uint8_t>& b) {

        return {get_RiffHeader(b), get_WaveFmtHeader(b), get_WaveDataHeader(b)};
    }

    void handleRiffHeaderExcpetions(const RiffHeader riff) {
        if (memcmp(RIFF_ID, riff.groupId, sizeof (riff.groupId)) != 0) {
            throw ParseWAVFileError(RIFF_ID_CHECK_FAILED);
        }

        if (memcmp(WAVE_ID, riff.riftType, sizeof (riff.riftType)) != 0) {
            throw ParseWAVFileError(WAVE_ID_CHECK_FAILED);
        }
    }

    const RiffHeader
    get_RiffHeader(const std::vector<uint8_t>& buffer) {

        if (buffer.size() < RIFF_FILE_HEADER_LENGTH) {
            throw ParseWAVFileError(BUFFERSIZE_INSUFFICIENT_FOR_RIFF_PARSING);
        }

        RiffHeader riff{
            {buffer[0], buffer[1], buffer[2], buffer[3]},
            (get_Value<uint32_t>(buffer, 4)),
            {buffer[8], buffer[9], buffer[10], buffer[11]}};

        handleRiffHeaderExcpetions(riff);
        return riff;
    }

    const std::size_t getWaveFmtStartPosition(const std::vector<uint8_t>& buffer) {
        //search for FORMAT_CHUNKID in buffer
        auto it = std::search(buffer.begin(), buffer.end(),
                FORMAT_CHUNKID.begin(), FORMAT_CHUNKID.end());
        if (it == buffer.end()) {
            throw ParseWAVFileError(FORMAT_ID_NOT_FOUND);
        }
        return std::distance(std::begin(buffer), it);
    }

    const WaveFmtHeader parseWaveFmtHeader(
            const std::vector<uint8_t>& buffer,
            const std::size_t start) {

        WaveFmtHeader wave;
        for (int i = 0; i < sizeof (wave.chunkId); i++) {
            wave.chunkId[i] = buffer[start + i];
        }

        //setting uint16_t values latter to avoid narrowing conversion warning in initialiser list
        wave.chunkSize = get_Value<uint32_t>(buffer, start + 4);
        wave.formatTag = get_Value<uint16_t>(buffer, start + 8);
        wave.channels = get_Value<uint16_t>(buffer, start + 10);
        wave.SamplesPerSec = get_Value<uint32_t>(buffer, start + 12);
        wave.bytesPerSecond = get_Value<uint32_t>(buffer, start + 16);
        wave.blockAlignment = get_Value<uint16_t>(buffer, start + 20);
        wave.bitsPerSample = get_Value<uint16_t>(buffer, start + 22);

        return wave;
    }

    void handleWaveFmtHeaderExceptions(const WaveFmtHeader& fmtHeader) {
        if (memcmp(FORMAT_ID, fmtHeader.chunkId, sizeof (fmtHeader.chunkId)) != 0) {
            throw ParseWAVFileError(FORMAT_ID_CHECK_FAILED);
        }

        //if formatTag not 0x1 then chunkSize may be greater than MIN_FORMAT_HEADER_LENGTH for additional information see http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
        if (fmtHeader.chunkSize < MIN_FORMAT_HEADER_LENGTH) {
            throw ParseWAVFileError(FORMAT_HEADER_LENGTH_CHECK_FAILED);
        }

        if (!((fmtHeader.formatTag == WAVE_FORMAT_PCM) |
                (fmtHeader.formatTag == WAVE_FORMAT_IEEE_FLOAT) |
                (fmtHeader.formatTag == WAVE_FORMAT_ALAW) |
                (fmtHeader.formatTag == WAVE_FORMAT_MULAW) |
                (fmtHeader.formatTag == WAVE_FORMAT_EXTENSIBLE))) {
            throw ParseWAVFileError(WAVE_FORMATTAG_NOT_SUPPORTED);
        }
    }

    const WaveFmtHeader
    get_WaveFmtHeader(const std::vector<uint8_t>& buffer) {

        if (buffer.size() < RIFF_WAV_FORMAT_HEADER_LENGTH) {
            throw ParseWAVFileError(BUFFERSIZE_INSUFFICIENT_FOR_FORMAT_PARSING);
        }

        auto start = getWaveFmtStartPosition(buffer);

        if ((buffer.size() - start) < RIFF_WAV_FORMAT_HEADER_LENGTH) {
            throw ParseWAVFileError(BUFFERSIZE_INSUFFICIENT_FOR_FORMAT_PARSING);
        }

        auto fmtHeader = parseWaveFmtHeader(buffer, start);

        handleWaveFmtHeaderExceptions(fmtHeader);

        return fmtHeader;
    }

    const bool containsDataId(const std::vector<uint8_t>& buffer) {
        auto it = std::search(buffer.begin(), buffer.end(), DATA_CHUNKID.begin(), DATA_CHUNKID.end());
        if (it == buffer.end()) {
            return false;
        }
        return true;
    }

    const std::size_t
    getDataStartPosition(const std::vector<uint8_t>& buffer) {
        //search for DATA_CHUNKID in buffer
        auto it = std::search(buffer.begin(), buffer.end(), DATA_CHUNKID.begin(), DATA_CHUNKID.end());
        if (it == buffer.end()) {
            throw ParseWAVFileError(DATA_ID_NOT_FOUND);
        }
        return std::distance(std::begin(buffer), it);
    }

    const WaveDataHeader parseWaveDataHeader(
            const std::vector<uint8_t>& buffer,
            const std::size_t start) {

        WaveDataHeader data;

        for (int i = 0; i < sizeof (data.chunkId); i++) {
            data.chunkId[i] = buffer[start + i];
        }
        data.dataSize = get_Value<uint32_t>(buffer, start + 4);

        return data;
    }

    void handleWaveDataHEaderException(const WaveDataHeader& data) {
        if (memcmp(DATA_ID, data.chunkId, sizeof (data.chunkId)) != 0) {
            throw ParseWAVFileError(DATA_ID_CHECK_FAILED);
        }
    }

    const WaveDataHeader
    get_WaveDataHeader(const std::vector<uint8_t>& buffer) {

        if (buffer.size() < RIFF_WAV_DATA_HEADER_LENGTH) {
            throw ParseWAVFileError(BUFFERSIZE_INSUFFICIENT_FOR_DATA_PARSING);
        }

        auto start = getDataStartPosition(buffer);

        if ((buffer.size() - start) < RIFF_WAV_DATA_HEADER_LENGTH) {
            throw ParseWAVFileError(BUFFERSIZE_INSUFFICIENT_FOR_DATA_PARSING);
        }

        auto waveDataHeader = parseWaveDataHeader(buffer, start);

        handleWaveDataHEaderException(waveDataHeader);
        return waveDataHeader;
    }
    
    const std::size_t 
    getSampleStartPosition(const std::vector<uint8_t>& buffer){
        return getDataStartPosition(buffer) + RIFF_WAV_DATA_HEADER_LENGTH;
    }

    void moveBufferToSamplePosition(std::vector<uint8_t>& buffer) {
        if (containsDataId) {
            auto start = getSampleStartPosition(buffer);
            buffer.erase(buffer.begin(), buffer.begin() + start);
        }
    }
    
    const std::size_t getDataSampleSize(const WAVE_Format& wave){
        if( wave.data.dataSize % wave.fmt.blockAlignment != 0){//data Size is missaligned
            throw ParseWAVFileError(DATASIZE_NOTALIGNED_WITH_BLOCKSIZE);
        }
        return wave.data.dataSize / wave.fmt.blockAlignment;
    }

}//end namespace