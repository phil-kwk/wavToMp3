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
 * File:   WAVFileBuffer.h
 * Author: Philipp Kaaden
 *
 * Created on 12. November 2020, 16:30
 */

#ifndef WAVFILEBUFFER_H
#define WAVFILEBUFFER_H

#include <cstdint>
#include <exception>
#include <vector>

#include "Formats.h"

namespace encoder {

    enum ParseWAVFileErrorCodes {
        BUFFERSIZE_INSUFFICIENT_FOR_RIFF_PARSING = 10,
        BUFFERSIZE_INSUFFICIENT_FOR_FORMAT_PARSING = 11,
        BUFFERSIZE_INSUFFICIENT_FOR_DATA_PARSING = 12,

        RIFF_ID_CHECK_FAILED = 20,
        WAVE_ID_CHECK_FAILED = 21,
        FORMAT_ID_NOT_FOUND = 22,
        FORMAT_ID_CHECK_FAILED = 23,
        DATA_ID_NOT_FOUND = 24,
        DATA_ID_CHECK_FAILED = 25,

        RIFF_FILE_LENGTH_CHECKFAILED = 30,
        FORMAT_HEADER_LENGTH_CHECK_FAILED = 31,

        WAVE_FORMATTAG_NOT_SUPPORTED = 40,

        DATA_LENGTH_BIGGER_THAN_BUFFER = 50,
        DATASIZE_NOTALIGNED_WITH_BLOCKSIZE = 51,
    };

    class ParseWAVFileError : std::exception {
    public:
        ParseWAVFileError(const ParseWAVFileErrorCodes msg);
        const ParseWAVFileErrorCodes code();
    private:
        ParseWAVFileErrorCodes errcode;
    };

    /**
     * try to Parse RIFF Header from Buffer
     * @param buffer containing RIFF Header
     * @return RiffHeader
     * @throw ParseWAVFileError
     */
    const RiffHeader
    get_RiffHeader(const std::vector<uint8_t>& buffer);

    /**
     * try to parse WaveFmtHeader from buffer
     * @param buffer containing WaveFmtHeader
     * @return WaveFmtHeader
     * @throw ParseWAVFileError
     */
    const WaveFmtHeader
    get_WaveFmtHeader(const std::vector<uint8_t>& buffer);

    /**
     * try to parse WaveDataHeader from Buffer
     * @param buffer containing WaveDataHeader
     * @return WaveDataHeader
     * @throw ParseWAVFileError
     */
    const WaveDataHeader
    get_WaveDataHeader(const std::vector<uint8_t>& buffer);

    /**
     * try to parse WAVE_Format from buffer
     * @param buffer containing WAVE_Format
     * @return WAVE_Format
     * @throw ParseWAVFileError
     */
    const WAVE_Format
    get_WaveFormat(const std::vector<uint8_t>& buffer);

    //deprecated will be removed once Unit Tests updated
    void moveBufferToSamplePosition(std::vector<uint8_t>& buffer);

    /**
     * getSampleStartPosition in Buffer
     * @param buffer containing WaveDataHeader
     * @return position
     * @throws ParseWAVFileError
     */
    const std::size_t getSampleStartPosition(const std::vector<uint8_t>& buffer);
    
    const std::size_t getDataSampleSize(const WAVE_Format& wave);

}//end namespace
#endif /* WAVFILEBUFFER_H */

