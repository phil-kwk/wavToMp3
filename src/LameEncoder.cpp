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
 * File:   LameEncoder.cpp
 * Author: Philipp Kaaden
 * 
 * Created on 16. November 2020, 15:16
 */

#include "LameEncoder.h"

#include <iostream>

namespace encoder {

    ExceptionLameEncoding::ExceptionLameEncoding(const ErrorCodeLameEncoding code) :
    errcode(code) {

    }

    const ErrorCodeLameEncoding ExceptionLameEncoding::msg() {
        return errcode;
    }

    template<typename T>
    LameEncoder<T>::LameEncoder() : lgf(lame_init()) {
        if (lgf == NULL) {
            throw ExceptionLameEncoding(LAME_INIT_FAILED);
        }
    }

    template LameEncoder<int16_t>::LameEncoder();
    template LameEncoder<int32_t>::LameEncoder();

    template<typename T>
    LameEncoder<T>::~LameEncoder() {
        lame_close(lgf);
    }

    template LameEncoder<int16_t>::~LameEncoder();
    template LameEncoder<int32_t>::~LameEncoder();

    void encodeResultHandler(int ret) {
        if (ret < 0) {
            if (ret == -1) {
                throw ExceptionLameEncoding(ENCODING_MP3_BUFFER_TO_SMALL);
            } else if (ret == -2) {
                throw ExceptionLameEncoding(ENCODING_MALLOC_PROBLEM);
            } else if (ret == -3) {
                throw ExceptionLameEncoding(ENCODING_LAME_INIT_NOT_CALLED);
            } else if (ret == -4) {
                throw ExceptionLameEncoding(ENCODING_PSYCHO_ACUSTIC_PROBLEM);
            } else {
                throw ExceptionLameEncoding(ENCODING_UNEXPECTED_PROBLEM);
            }
        }
    }

    template<>
    const std::vector<uint8_t>
    LameEncoder<int16_t>::encodePCMSamples(
            const ChannelContainer<int16_t>& samples) {

        const std::size_t numberOfSamples = samples.at(0).size();
        const std::size_t numberChannels = samples.size();
        const std::size_t mp3BufferSizeMax = 1.25 * numberOfSamples + 7200;

        std::vector<uint8_t> mp3Buffer;
        mp3Buffer.resize(mp3BufferSizeMax); //resize is used because lame_encode_buffer will access by &[]

        int ret = 0;
        if (numberChannels == 2) {
            ret = lame_encode_buffer(lgf,
                    &samples.at(0)[0],
                    &samples.at(1)[0],
                    numberOfSamples,
                    &mp3Buffer[0],
                    mp3Buffer.size());
        } else {
            throw ExceptionLameEncoding(NUMBER_CHANNELS_NOT_SUPOORTED);
        }

        encodeResultHandler(ret);
        mp3Buffer.resize(ret); //MP3 Buffer will be resized to the number of mp3 Bytes actually encoded
        return mp3Buffer;
    }

    template<>
    const std::vector<uint8_t>
    LameEncoder<int32_t>::encodePCMSamples(
            const ChannelContainer<int32_t>& samples) {

        const std::size_t numberOfSamples = samples.at(0).size();
        const std::size_t numberChannels = samples.size();
        const std::size_t mp3BufferSizeMax = 1.25 * numberOfSamples + 7200;

        std::vector<uint8_t> mp3Buffer;
        mp3Buffer.resize(mp3BufferSizeMax); //resize is used because lame_encode_buffer will access by &[]

        int ret = 0;
        if (numberChannels == 2) {
            ret = lame_encode_buffer_int(lgf,
                    &samples.at(0)[0],
                    &samples.at(1)[0],
                    numberOfSamples,
                    &mp3Buffer[0],
                    mp3Buffer.size());
        } else {
            throw ExceptionLameEncoding(NUMBER_CHANNELS_NOT_SUPOORTED);
        }

        encodeResultHandler(ret);
        mp3Buffer.resize(ret); //MP3 Buffer will be resized to the number of mp3 Bytes actually encoded
        return mp3Buffer;
    }

    template<typename T>
    const std::vector<uint8_t>
    LameEncoder<T>::encode_flush(const bool noGap) {
        std::vector<uint8_t> flushBuffer;
        flushBuffer.resize(7200);

        int ret = 0;
        if (noGap) {
            ret = lame_encode_flush_nogap(lgf, &flushBuffer[0], flushBuffer.size());
        } else {
            ret = lame_encode_flush(lgf, &flushBuffer[0], flushBuffer.size());
        }
        if (ret < 0) {
            throw ExceptionLameEncoding(ENCODING_FLUSH_PROBLEM);
        }

        flushBuffer.resize(ret);
        return flushBuffer;
    }

    template const std::vector<uint8_t> LameEncoder<int16_t>::encode_flush(const bool noGap);
    template const std::vector<uint8_t> LameEncoder<int32_t>::encode_flush(const bool noGap);

    template<typename T>
    void LameEncoder<T>::setConfig(
            const WaveFmtHeader conf,
            const LameEncodingQuality quality) {

        int ret = 0;
        ret = lame_set_in_samplerate(lgf, conf.SamplesPerSec);
        if (ret < 0) {
            throw ExceptionLameEncoding(CONFIG_SET_SAMPLERATE_PROBLEM);
        }

        ret = lame_set_num_channels(lgf, conf.channels);
        if (ret < 0) {
            throw ExceptionLameEncoding(CONFIG_SET_CHANNELS_PROBLEM);
        }

        ret = lame_set_quality(lgf, quality);
        if (ret < 0) {
            throw ExceptionLameEncoding(CONFIG_SET_QUALITY_PROBLEM);
        }

        ret = lame_init_params(lgf);
        if (ret < 0) {
            throw ExceptionLameEncoding(CONFIG_INIT_PARAMS_PROBLEM);
        }
    }

    template void LameEncoder<int16_t>::setConfig(const WaveFmtHeader conf, const LameEncodingQuality quality);
    template void LameEncoder<int32_t>::setConfig(const WaveFmtHeader conf, const LameEncodingQuality quality);

}//end namespace