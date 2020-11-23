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
 * File:   LameEncoder.h
 * Author: Philipp Kaaden
 *
 * Created on 16. November 2020, 15:15
 */

#ifndef LAMEENCODER_H
#define LAMEENCODER_H

#include <exception>
#include <cstdint>
#include <vector>

#include <lame.h>

#include "Formats.h"

namespace encoder {

    enum LameEncodingQuality {
        absolut_best = 0,
        near_best = 2,
        good = 5,
        ok = 7,
        bad = 9
    };

    enum ErrorCodeLameEncoding {
        LAME_INIT_FAILED = 10,

        ENCODING_MP3_BUFFER_TO_SMALL = 40,
        ENCODING_MALLOC_PROBLEM = 41,
        ENCODING_LAME_INIT_NOT_CALLED = 42,
        ENCODING_PSYCHO_ACUSTIC_PROBLEM = 43,
        ENCODING_UNEXPECTED_PROBLEM = 44,
        ENCODING_FLUSH_PROBLEM = 45,

        STEREO_BUFFER_LEFT_RIGHT_NOTEQUAL = 50,

        CONFIG_INIT_PARAMS_PROBLEM = 60,
        CONFIG_SET_SAMPLERATE_PROBLEM = 61,
        CONFIG_SET_QUALITY_PROBLEM = 62,
        CONFIG_SET_CHANNELS_PROBLEM = 63,
        
        NUMBER_CHANNELS_NOT_SUPOORTED = 70
    };

    class ExceptionLameEncoding : std::exception {
    public:
        ExceptionLameEncoding(const ErrorCodeLameEncoding code);
        const ErrorCodeLameEncoding msg();
    private:
        const ErrorCodeLameEncoding errcode;
    };

    template<typename SampleTyp>
    class LameEncoder {
    public:
        LameEncoder();
        virtual ~LameEncoder();

        const std::vector<uint8_t> encodePCMSamples(
                const ChannelContainer<SampleTyp>& samples);

        const std::vector<uint8_t> encode_flush(const bool noGap);

        void setConfig(
                const WaveFmtHeader conf,
                const LameEncodingQuality quality);
    private:
        lame_global_flags * lgf;
    };


}//end namespace
#endif /* LAMEENCODER_H */