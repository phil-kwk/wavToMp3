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
 * File:   Converter.cpp
 * Author: Philipp Kaaden
 * 
 * Created on 19. November 2020, 07:09
 */

#include "SampleConverter.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstring>

namespace encoder {

    ExceptionConverter::ExceptionConverter(const CodeExceptionConverter code) :
    errcode(code) {

    }

    const CodeExceptionConverter ExceptionConverter::code() {
        return errcode;
    }


    namespace {
        const uint32_t UINT8_SIZE = 1;
        const uint32_t INT16_SIZE = 2;
        const uint32_t INT24_SIZE = 3;
        const uint32_t INT32_SIZE = 4;

        const uint32_t MONO_CHANNELS = 1;
        const uint32_t STEREO_CHANNELS = 2;

        const uint32_t UINT8_MONO_BLOCK_ALLIGNMENT = UINT8_SIZE*MONO_CHANNELS;
        const uint32_t UINT8_STEREO_BLOCK_ALLIGNMENT = UINT8_SIZE*STEREO_CHANNELS;

        const uint32_t INT16_MONO_BLOCK_ALLIGNMENT = INT16_SIZE*MONO_CHANNELS;
        const uint32_t INT16_STEREO_BLOCK_ALLIGNMENT = INT16_SIZE*STEREO_CHANNELS;

        const uint32_t INT24_MONO_BLOCK_ALLIGNMENT = INT24_SIZE*MONO_CHANNELS;
        const uint32_t INT24_STEREO_BLOCK_ALLIGNMENT = INT24_SIZE*STEREO_CHANNELS;

        const uint32_t INT32_MONO_BLOCK_ALLIGNMENT = INT32_SIZE*MONO_CHANNELS;
        const uint32_t INT32_STEREO_BLOCK_ALLIGNMENT = INT32_SIZE*STEREO_CHANNELS;


        const uint32_t PCM_SAMPLES_IN_MP3_FRAME = 1152;

        template<typename T>
        const std::vector< std::vector<T> > slice_Multiple1152(
                const std::vector<T>& elements,
                const std::size_t buckets) {

            const std::size_t Number = elements.size();
            const std::size_t Multiple = PCM_SAMPLES_IN_MP3_FRAME;

            std::vector<std::vector<T> > slices;
            if (Number <= PCM_SAMPLES_IN_MP3_FRAME) {
                std::vector<T> s;
                s.insert(
                        s.end(),
                        elements.begin(),
                        elements.end());
                slices.push_back(s);
                return slices;
            }

            const std::size_t NperM = floor(Number / Multiple);
            //            const std::size_t NRestM = Number % Multiple;
            const std::size_t NperMperB = floor(NperM / buckets);
            //            const std::size_t BRest = NperM % buckets;

            if (NperMperB < 1) {
                return slice_Multiple1152<T>(elements, buckets - 1); //try with less Buckets
            }

            std::size_t sliced = 0;
            for (std::size_t n = 0; n < buckets; n++) {
                std::vector<T> s;
                s.insert(s.end(),
                        elements.begin() + sliced,
                        elements.begin() + sliced + NperMperB * Multiple);
                sliced += NperMperB * Multiple;
                if ((n + 1) == buckets) {//add rest to last Bucket
                    s.insert(s.end(),
                            elements.begin() + sliced,
                            elements.end());
                }
                slices.push_back(s);
            }
            return slices;
        }

        template<typename SampleTyp>
        const SampleTyp get_SampleValue(
                const std::vector<uint8_t>& buffer,
                const std::size_t position);

        template<>
        const uint8 get_SampleValue<uint8>(
                const std::vector<uint8_t>& buffer,
                const std::size_t position) {
            uint8_t uv = buffer.at(position);
            int8_t sv;
            memcpy(&sv, &uv, sizeof(uv));
            
            uint8 val;
            val.data = sv * 256;
            return val;
        }

        template<>
        const int16_t get_SampleValue<int16_t>(
                const std::vector<uint8_t>& buffer,
                const std::size_t position) {
            return int16_t(buffer.at(position + 1) << 8) | int16_t(buffer.at(position));
        }

        template<>
        const int24 get_SampleValue<int24>(
                const std::vector<uint8_t>& buffer,
                const std::size_t position) {
            int24 val;
            val.data = (int32_t(buffer.at(position + 2) << 16) | int32_t(buffer.at(position + 1) << 8) | int32_t(buffer.at(position))) * 256;
            return val;
        }

        template<>
        const int32_t get_SampleValue<int32_t>(
                const std::vector<uint8_t>& buffer,
                const std::size_t position) {
            return int32_t(buffer.at(position + 3) << 24) | int32_t(buffer.at(position + 2) << 16) |
                    int32_t(buffer.at(position + 1) << 8) | int32_t(buffer.at(position));
        }

        template<>
        const float32 get_SampleValue<float32>(
                const std::vector<uint8_t>& buffer,
                const std::size_t position) {
            float32 val;
            for (std::size_t n = 0; n < 4; n++) {
                val.data[n] = buffer.at(position + n);
            }
            return val;
        }

        template<>
        const float64 get_SampleValue<float64>(
                const std::vector<uint8_t>& buffer,
                const std::size_t position) {
            float64 val;
            for (std::size_t n = 0; n < 8; n++) {
                val.data[n] = buffer.at(position + n);
            }
            return val;
        }
    }

    template<typename SampleTyp>
    const ChannelContainer<SampleTyp> get_pcm_Samples(
            const std::vector<uint8_t>& buffer,
            const std::size_t numberOfSamples,
            const std::size_t numberChannels) {

        if (buffer.size() < numberOfSamples * sizeof (SampleTyp) * numberChannels) {
            throw ExceptionConverter(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES);
        }

        ChannelContainer<SampleTyp> cspcm;
        if (numberOfSamples == 0 ||
                buffer.size() == 0 ||
                numberChannels == 0) {
            return cspcm;
        }

        for (std::size_t c = 0; c < numberChannels; c++) {
            std::vector<SampleTyp> channel;
            for (std::size_t pos = 0;
                    pos < (numberOfSamples * sizeof (SampleTyp) * numberChannels);
                    pos = pos + sizeof (SampleTyp) * numberChannels) {
                auto value = get_SampleValue<SampleTyp>(buffer, pos + c * sizeof (SampleTyp));
                channel.push_back(value);
            }
            cspcm.push_back(channel);
        }
        return cspcm;
    }
    template const ChannelContainer<int16_t> get_pcm_Samples<int16_t>(const std::vector<uint8_t>& samples, const std::size_t numberOfSamples, const std::size_t numberChannels);
    template const ChannelContainer<int32_t> get_pcm_Samples<int32_t>(const std::vector<uint8_t>& samples, const std::size_t numberOfSamples, const std::size_t numberChannels);
    template const ChannelContainer<float32> get_pcm_Samples<float32>(const std::vector<uint8_t>& samples, const std::size_t numberOfSamples, const std::size_t numberChannels);
    template const ChannelContainer<float64> get_pcm_Samples<float64>(const std::vector<uint8_t>& samples, const std::size_t numberOfSamples, const std::size_t numberChannels);

    template<>
    const ChannelContainer<uint8> get_pcm_Samples<uint8> (
            const std::vector<uint8_t>& buffer,
            const std::size_t numberOfSamples,
            const std::size_t numberChannels) {

        const std::size_t sizeof_uint8 = UINT8_SIZE;

        if (buffer.size() < numberOfSamples * sizeof_uint8 * numberChannels) {
            throw ExceptionConverter(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES);
        }

        ChannelContainer<uint8> cspcm;
        if (numberOfSamples == 0 ||
                buffer.size() == 0 ||
                numberChannels == 0) {
            return cspcm;
        }

        for (std::size_t c = 0; c < numberChannels; c++) {
            std::vector<uint8> channel;
            for (std::size_t pos = 0;
                    pos < (numberOfSamples * sizeof_uint8 * numberChannels);
                    pos = pos + sizeof_uint8 * numberChannels) {
                auto value = get_SampleValue<uint8>(buffer, pos + c * sizeof_uint8);
                channel.push_back(value);
            }
            cspcm.push_back(channel);
        }
        return cspcm;
    }
    
    template<>
    const ChannelContainer<int24> get_pcm_Samples<int24> (
            const std::vector<uint8_t>& buffer,
            const std::size_t numberOfSamples,
            const std::size_t numberChannels) {

        const std::size_t sizeof_int24 = INT24_SIZE;

        if (buffer.size() < numberOfSamples * sizeof_int24 * numberChannels) {
            throw ExceptionConverter(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES);
        }

        ChannelContainer<int24> cspcm;
        if (numberOfSamples == 0 ||
                buffer.size() == 0 ||
                numberChannels == 0) {
            return cspcm;
        }

        for (std::size_t c = 0; c < numberChannels; c++) {
            std::vector<int24> channel;
            for (std::size_t pos = 0;
                    pos < (numberOfSamples * sizeof_int24 * numberChannels);
                    pos = pos + sizeof_int24 * numberChannels) {
                auto value = get_SampleValue<int24>(buffer, pos + c * sizeof_int24);
                channel.push_back(value);
            }
            cspcm.push_back(channel);
        }
        return cspcm;
    }

    template<typename T>
    const bool checkChannelSampleSizeEqual(
            const ChannelContainer<T>& channelContainer) {

        std::vector<std::size_t> sampleSizes;

        for (Channel<T> c : channelContainer) {
            sampleSizes.push_back(c.size());
        }

        if (std::adjacent_find(sampleSizes.begin(), sampleSizes.end(), std::not_equal_to<std::size_t>()) == sampleSizes.end()) {
            return true;
        }
        return false;
    }
    template const bool checkChannelSampleSizeEqual(const ChannelContainer<uint8>& channelContainer);
    template const bool checkChannelSampleSizeEqual(const ChannelContainer<int16_t>& channelContainer);
    template const bool checkChannelSampleSizeEqual(const ChannelContainer<int24>& channelContainer);
    template const bool checkChannelSampleSizeEqual(const ChannelContainer<int32_t>& channelContainer);
    template const bool checkChannelSampleSizeEqual(const ChannelContainer<float32>& channelContainer);
    template const bool checkChannelSampleSizeEqual(const ChannelContainer<float64>& channelContainer);

    template<typename T>
    const SlicedChannelContainer<T> slice_PCMSamples(
            const ChannelContainer<T>& channelContainer,
            const uint8_t buckets) {

        if (!checkChannelSampleSizeEqual(channelContainer)) {
            throw ExceptionConverter(SAMPLESIZE_OF_CHANNELS_NOTEQUAL);
        }

        const std::size_t numberChannels = channelContainer.size();
        SlicedChannelContainer<T> scc;

        if (numberChannels < 1) {//no channels in container return empty slicedChannelContainer
            return scc;
        }

        const std::size_t sampleSize = channelContainer.at(0).size();

        if (buckets < 2 || sampleSize < PCM_SAMPLES_IN_MP3_FRAME) {
            scc.push_back(channelContainer);
            return scc;
        }

        scc.resize(buckets); //

        std::size_t numberSlices = 0;
        for (std::size_t c = 0; c < numberChannels; c++) {
            Channel<T> channel = channelContainer.at(c);
            std::vector< Channel<T> > channelSlice =
                    slice_Multiple1152<T>(channel, buckets);

            numberSlices = channelSlice.size();
            for (std::size_t b = 0; b < numberSlices; b++) {
                scc.at(b).push_back(channelSlice.at(b));
            }
        }
        scc.resize(numberSlices); //
        return scc;
    }
    template const std::vector<ChannelContainer<uint8>> slice_PCMSamples <uint8>(const ChannelContainer<uint8>& samples, const uint8_t buckets);
    template const std::vector<ChannelContainer<int16_t>> slice_PCMSamples <int16_t>(const ChannelContainer<int16_t>& samples, const uint8_t buckets);
    template const std::vector<ChannelContainer<int24>> slice_PCMSamples <int24>(const ChannelContainer<int24>& samples, const uint8_t buckets);
    template const std::vector<ChannelContainer<int32_t>> slice_PCMSamples <int32_t>(const ChannelContainer<int32_t>& samples, const uint8_t buckets);
    template const std::vector<ChannelContainer<float32>> slice_PCMSamples <float32>(const ChannelContainer<float32>& samples, const uint8_t buckets);
    template const std::vector<ChannelContainer<float64>> slice_PCMSamples <float64>(const ChannelContainer<float64>& samples, const uint8_t buckets);

    template<typename T>
    const ChannelContainer<T> combine_PCMSamples(
            const SlicedChannelContainer<T>& scc) {

        ChannelContainer<T> buffer;

        if (scc.size() < 1) {
            buffer;
        }

        const int numberChannels = scc.at(0).size();
        buffer.resize(numberChannels);

        for (std::size_t i = 0; i < numberChannels; i++) {
            for (auto cc : scc) {
                buffer.at(i).insert(buffer.at(i).end(), cc.at(i).begin(), cc.at(i).end());
            }
        }
        return buffer;
    }
    template const ChannelContainer<uint8> combine_PCMSamples<uint8> (const SlicedChannelContainer<uint8>& scc);
    template const ChannelContainer<int16_t> combine_PCMSamples<int16_t> (const SlicedChannelContainer<int16_t>& scc);
    template const ChannelContainer<int24> combine_PCMSamples<int24> (const SlicedChannelContainer<int24>& scc);
    template const ChannelContainer<int32_t> combine_PCMSamples<int32_t> (const SlicedChannelContainer<int32_t>& scc);
    template const ChannelContainer<float32> combine_PCMSamples<float32> (const SlicedChannelContainer<float32>& scc);
    template const ChannelContainer<float64> combine_PCMSamples<float64> (const SlicedChannelContainer<float64>& scc);
}//end namespace