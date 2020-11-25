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
#include <map>

namespace encoder {

    namespace {

        std::map<uint8_t, int16_t> uint8_int16_mapping = {
            {0,-32768},{1,-32512},{2,-32256},{3,-32000},{4,-31744},{5,-31488},{6,-31232},{7,-30976},{8,-30720},{9,-30464},{10,-30208},{11,-29952},{12,-29696},{13,-29440},{14,-29184},{15,-28928},{16,-28672},{17,-28416},{18,-28160},{19,-27904},{20,-27648},{21,-27392},{22,-27136},{23,-26880},{24,-26624},{25,-26368},{26,-26112},{27,-25856},{28,-25600},{29,-25344},{30,-25088},{31,-24832},{32,-24576},{33,-24320},{34,-24064},{35,-23808},{36,-23552},{37,-23296},{38,-23040},{39,-22784},{40,-22528},{41,-22272},{42,-22016},{43,-21760},{44,-21504},{45,-21248},{46,-20992},{47,-20736},{48,-20480},{49,-20224},{50,-19968},{51,-19712},{52,-19456},{53,-19200},{54,-18944},{55,-18688},{56,-18432},{57,-18176},{58,-17920},{59,-17664},{60,-17408},{61,-17152},{62,-16896},{63,-16640},{64,-16384},{65,-16128},{66,-15872},{67,-15616},{68,-15360},{69,-15104},{70,-14848},{71,-14592},{72,-14336},{73,-14080},{74,-13824},{75,-13568},{76,-13312},{77,-13056},{78,-12800},{79,-12544},{80,-12288},{81,-12032},{82,-11776},{83,-11520},{84,-11264},{85,-11008},{86,-10752},{87,-10496},{88,-10240},{89,-9984},{90,-9728},{91,-9472},{92,-9216},{93,-8960},{94,-8704},{95,-8448},{96,-8192},{97,-7936},{98,-7680},{99,-7424},{100,-7168},{101,-6912},{102,-6656},{103,-6400},{104,-6144},{105,-5888},{106,-5632},{107,-5376},{108,-5120},{109,-4864},{110,-4608},{111,-4352},{112,-4096},{113,-3840},{114,-3584},{115,-3328},{116,-3072},{117,-2816},{118,-2560},{119,-2304},{120,-2048},{121,-1792},{122,-1536},{123,-1280},{124,-1024},{125,-768},{126,-512},{127,-256},{128,0},{129,256},{130,512},{131,768},{132,1024},{133,1280},{134,1536},{135,1792},{136,2048},{137,2304},{138,2560},{139,2816},{140,3072},{141,3328},{142,3584},{143,3840},{144,4096},{145,4352},{146,4608},{147,4864},{148,5120},{149,5376},{150,5632},{151,5888},{152,6144},{153,6400},{154,6656},{155,6912},{156,7168},{157,7424},{158,7680},{159,7936},{160,8192},{161,8448},{162,8704},{163,8960},{164,9216},{165,9472},{166,9728},{167,9984},{168,10240},{169,10496},{170,10752},{171,11008},{172,11264},{173,11520},{174,11776},{175,12032},{176,12288},{177,12544},{178,12800},{179,13056},{180,13312},{181,13568},{182,13824},{183,14080},{184,14336},{185,14592},{186,14848},{187,15104},{188,15360},{189,15616},{190,15872},{191,16128},{192,16384},{193,16640},{194,16896},{195,17152},{196,17408},{197,17664},{198,17920},{199,18176},{200,18432},{201,18688},{202,18944},{203,19200},{204,19456},{205,19712},{206,19968},{207,20224},{208,20480},{209,20736},{210,20992},{211,21248},{212,21504},{213,21760},{214,22016},{215,22272},{216,22528},{217,22784},{218,23040},{219,23296},{220,23552},{221,23808},{222,24064},{223,24320},{224,24576},{225,24832},{226,25088},{227,25344},{228,25600},{229,25856},{230,26112},{231,26368},{232,26624},{233,26880},{234,27136},{235,27392},{236,27648},{237,27904},{238,28160},{239,28416},{240,28672},{241,28928},{242,29184},{243,29440},{244,29696},{245,29952},{246,30208},{247,30464},{248,30720},{249,30976},{250,31232},{251,31488},{252,31744},{253,32000},{254,32256},{255,32512}
        };

        std::vector<int16_t> conversion(const std::vector<uint8_t> elements) {
            std::vector<int16_t> converted;
            for( auto e : elements){
                converted.push_back(uint8_int16_mapping[e]);
            }
            return converted;
        }

    }

    ExceptionLameEncoding::ExceptionLameEncoding(const ErrorCodeLameEncoding code) :
    errcode(code) {

    }

    const ErrorCodeLameEncoding ExceptionLameEncoding::code() {
        return errcode;
    }

    template<typename T>
    LameEncoder<T>::LameEncoder() : lgf(lame_init()) {
        if (lgf == NULL) {
            throw ExceptionLameEncoding(LAME_INIT_FAILED);
        }
    }
    template LameEncoder<uint8_t>::LameEncoder();
    template LameEncoder<int16_t>::LameEncoder();
    template LameEncoder<int24_t>::LameEncoder();
    template LameEncoder<int32_t>::LameEncoder();

    template<typename T>
    LameEncoder<T>::~LameEncoder() {
        lame_close(lgf);
    }
    template LameEncoder<uint8_t>::~LameEncoder();
    template LameEncoder<int16_t>::~LameEncoder();
    template LameEncoder<int24_t>::~LameEncoder();
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
    LameEncoder<uint8_t>::encodePCMSamples(
            const ChannelContainer<uint8_t>& samples) {

        const std::size_t numberOfSamples = samples.at(0).size();
        const std::size_t numberChannels = samples.size();
        const std::size_t mp3BufferSizeMax = 1.25 * numberOfSamples + 7200;

        std::vector<uint8_t> mp3Buffer;
        mp3Buffer.resize(mp3BufferSizeMax); //resize is used because lame_encode_buffer will access by &[]



        int ret = 0;
        if (numberChannels == 2) {
            ret = lame_encode_buffer(lgf,
                    & conversion(samples.at(0))[0],
                    & conversion(samples.at(1))[0],
                    numberOfSamples,
                    &mp3Buffer[0],
                    mp3Buffer.size());
        } else if (numberChannels == 1) {
            ret = lame_encode_buffer(lgf,
                    &conversion(samples.at(0))[0],
                    &conversion(samples.at(0))[0],
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
        } else if (numberChannels == 1) {
            ret = lame_encode_buffer(lgf,
                    &samples.at(0)[0],
                    &samples.at(0)[0],
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
    LameEncoder<int24_t>::encodePCMSamples(
            const ChannelContainer<int24_t>& samples) {

        const std::size_t numberOfSamples = samples.at(0).size();
        const std::size_t numberChannels = samples.size();
        const std::size_t mp3BufferSizeMax = 1.25 * numberOfSamples + 7200;

        std::vector<uint8_t> mp3Buffer;
        mp3Buffer.resize(mp3BufferSizeMax); //resize is used because lame_encode_buffer will access by &[]

        int ret = 0;
        if (numberChannels == 2) {
            ret = lame_encode_buffer_int(lgf,
                    &samples.at(0)[0].data,
                    &samples.at(1)[0].data,
                    numberOfSamples,
                    &mp3Buffer[0],
                    mp3Buffer.size());
        } else if (numberChannels == 1) {
            ret = lame_encode_buffer_int(lgf,
                    &samples.at(0)[0].data,
                    &samples.at(0)[0].data,
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
        } else if (numberChannels == 1) {
            ret = lame_encode_buffer_int(lgf,
                    &samples.at(0)[0],
                    &samples.at(0)[0],
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
    template const std::vector<uint8_t> LameEncoder<uint8_t>::encode_flush(const bool noGap);
    template const std::vector<uint8_t> LameEncoder<int16_t>::encode_flush(const bool noGap);
    template const std::vector<uint8_t> LameEncoder<int24_t>::encode_flush(const bool noGap);
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
    template void LameEncoder<uint8_t>::setConfig(const WaveFmtHeader conf, const LameEncodingQuality quality);
    template void LameEncoder<int16_t>::setConfig(const WaveFmtHeader conf, const LameEncodingQuality quality);
    template void LameEncoder<int24_t>::setConfig(const WaveFmtHeader conf, const LameEncodingQuality quality);
    template void LameEncoder<int32_t>::setConfig(const WaveFmtHeader conf, const LameEncodingQuality quality);

}//end namespace