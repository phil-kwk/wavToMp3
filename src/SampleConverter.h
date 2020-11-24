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
 * File:   Converter.h
 * Author: Philipp Kaaden
 *
 * Created on 19. November 2020, 07:09
 */

#ifndef PCM_SAMPLE_SPLITTING_H
#define PCM_SAMPLE_SPLITTING_H

#include "Formats.h"

#include <cstdint>
#include <cfloat>
#include <vector>
#include <exception>

namespace encoder {

    enum CodeExceptionConverter {
        BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES = 10,
        CHANNEL_TYPE_NOT_SUPPORTED = 20,
        SAMPLESIZE_OF_CHANNELS_NOTEQUAL = 30,
    };

    class ExceptionConverter : std::exception {
    public:
        ExceptionConverter(const CodeExceptionConverter code);
        const CodeExceptionConverter code();
    private:
        const CodeExceptionConverter errcode;
    };

    /**
     * @param buffer containing first sample at first position
     * @param numberSamples per channel
     * @param numberChannels in buffer
     * @return ChannelContainer<SampleType>
     * @throws Exception_PCM_Sample_Slicer(CHANNEL_TYPE_NOT_SUPPORTED)
     */
    template<typename SampleType>
    const ChannelContainer<SampleType> get_pcm_Samples(
            const std::vector<uint8_t>& buffer,
            const std::size_t numberSamples,
            const std::size_t numberChannels);

    /**
     * slices the PCMSamples into numberOfSlices with Multiple of 1152 
     * if samples <= 1152 only one slice is returned
     * used for getting slices of PCMSamples for parallel encoding by multiple threads
     * @param samples (ChannelContainer for Slicing)
     * @param numBuckets (number of Buckets to Slice the samples into)
     * @return slices (as multiples of 1152 and rest in last slice)
     * @throw Exception_PCM_Sample_Slicer
     */
    template<typename SampleType>
    const SlicedChannelContainer<SampleType> slice_PCMSamples(
            const ChannelContainer<SampleType>& samples,
            const uint8_t numBuckets);

    /**
     * combine a SlicedChannelContainer into a ChannelContainer
     * @param scc
     * @return 
     */
    template<typename T>
    const ChannelContainer<T> combine_PCMSamples(
            const SlicedChannelContainer<T>& scc);

    /**
     * check that all channels in ChannelContainer are of the same size
     * @param channelContainer
     * @return 
     */
    template<typename SampleType>
    const bool checkChannelSampleSizeEqual(
            const ChannelContainer<SampleType>& channelContainer);

}//end namespace
#endif /* PCM_SAMPLE_SPLITTING_H */

