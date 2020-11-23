/* 
 * File:   ConverterTest
 * Author: Philipp Kaaden
 *
 * Created on 19.11.2020, 07:57:31
 */

#include <algorithm>

#include <gtest/gtest.h>
#include <cmath>

#include "../src/SampleConverter.h"

#include "TestWaveData.h"

namespace encoder {

    class ConverterTest : public testing::Test {
    protected:
    };

    TEST_F(ConverterTest, call_get_pcm_Samples_uint8__Empty) {
        auto buffer = get_pcm_Samples<uint8_t>({}, 0, 2);
        EXPECT_EQ(0, buffer.size());
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int16__Empty) {
        auto buffer = get_pcm_Samples<int16_t>({}, 0, 2);
        EXPECT_EQ(0, buffer.size());
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int32__Empty) {
        auto buffer = get_pcm_Samples<int32_t>({}, 0, 2);
        EXPECT_EQ(0, buffer.size());
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int16__buffer) {
        auto wave = testfile_WAVE_FORMAT();
        auto buffer = testfile_Buffer();
        auto size = buffer.size();
        buffer.erase(buffer.begin(), buffer.begin() + 44);
        EXPECT_EQ(size, buffer.size() + 44);

        const int numberSamples = wave.data.dataSize / wave.fmt.blockAlignment;
        auto pcmSamples = get_pcm_Samples<int16_t>(buffer, numberSamples, 2);

        EXPECT_EQ(pcmSamples.at(0).size(), numberSamples);
        EXPECT_EQ(pcmSamples.at(0), testfile_left_samples());
        EXPECT_EQ(pcmSamples.at(1), testfile_right_samples());
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_uint8_errorBufferSize1) {
        try {
            get_pcm_Samples<uint8_t>({}, 1, 2);
            FAIL();
        } catch (ExceptionConverter exc) {
            EXPECT_EQ(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES, exc.code());
        }
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int16_errorBufferSize1) {
        try {
            get_pcm_Samples<int16_t>({}, 1, 2);
            FAIL();
        } catch (ExceptionConverter exc) {
            EXPECT_EQ(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES, exc.code());
        }
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int32_errorBufferSize1) {
        try {
            get_pcm_Samples<int32_t>({}, 1, 2);
            FAIL();
        } catch (ExceptionConverter exc) {
            EXPECT_EQ(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES, exc.code());
        }
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_uint8_errorBufferSize2) {
        try {
            get_pcm_Samples<uint8_t>({1}, 1, 2);
            FAIL();
        } catch (ExceptionConverter exc) {
            EXPECT_EQ(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES, exc.code());
        }
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int16_errorBufferSize2) {
        try {
            get_pcm_Samples<int16_t>({1, 2, 3}, 1, 2);
            FAIL();
        } catch (ExceptionConverter exc) {
            EXPECT_EQ(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES, exc.code());
        }
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int32_errorBufferSize2) {
        try {
            get_pcm_Samples<int32_t>({1, 2, 3, 4, 5, 6, 7}, 1, 2);
            FAIL();
        } catch (ExceptionConverter exc) {
            EXPECT_EQ(BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES, exc.code());
        }
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_uint8) {
        ChannelContainer<uint8_t> sample;
        auto slices = slice_PCMSamples<uint8_t>(sample, 0);
        EXPECT_EQ(0, slices.size());
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_int16) {
        ChannelContainer<int16_t> sample;
        auto slices = slice_PCMSamples<int16_t>(sample, 0);
        EXPECT_EQ(0, slices.size());
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_int32) {
        ChannelContainer<int32_t> sample;
        auto slices = slice_PCMSamples<int32_t>(sample, 0);
        EXPECT_EQ(0, slices.size());
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_oneSample_uint8) {
        ChannelContainer<uint8_t> container{
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 10, 11, UINT8_MAX}};

        std::vector<ChannelContainer < uint8_t>> scc =
                slice_PCMSamples<uint8_t>(container, 1);
        EXPECT_EQ(scc.at(0), container);
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_oneSample_int16) {
        ChannelContainer<int16_t> container{
            {1, 2, 3, INT16_MAX},
            {5, 6, INT16_MIN, 8},
            {9, 10, 11, 12}};

        std::vector<ChannelContainer < int16_t>> scc =
                slice_PCMSamples<int16_t>(container, 1);
        EXPECT_EQ(1, scc.size());
        EXPECT_EQ(scc.at(0), container);
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_oneSample_int32) {
        ChannelContainer<int32_t> container{
            {1, 2, 3, INT32_MAX},
            {5, 6, INT32_MIN, 8},
            {9, 10, 11, 12}};
        auto scc = slice_PCMSamples<int32_t>(container, 1);
        EXPECT_EQ(1, scc.size());
        EXPECT_EQ(scc.at(0), container);
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_B2) {
        ChannelContainer<int16_t> container{
            {1, 2, 3, INT16_MAX},
            {5, 6, INT16_MIN, 8},
            {9, 10, 11, 12}};
        auto scc = slice_PCMSamples<int16_t>(container, 2);
        EXPECT_EQ(1, scc.size());
        EXPECT_EQ(scc.at(0), container);
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_B2_int32) {
        ChannelContainer<int32_t> container{
            {1, 2, 3, INT32_MAX},
            {5, 6, INT32_MIN, 8},
            {9, 10, 11, 12}};
        auto scc = slice_PCMSamples<int32_t>(container, 2);
        EXPECT_EQ(1, scc.size());
        EXPECT_EQ(scc.at(0), container);
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_B3_int32) {
        ChannelContainer<int32_t> container{
            {1, 2, 3, INT32_MAX},
            {5, 6, INT32_MIN, 8},
            {9, 10, 11, 12}};
        auto scc = slice_PCMSamples<int32_t>(container, 3);
        EXPECT_EQ(1, scc.size());
        EXPECT_EQ(scc.at(0), container);


    }

    TEST_F(ConverterTest, call_slice_PCMSamples_ManySamples_B1) {
        ChannelContainer<int16_t> container{ testfile_left_samples(), testfile_right_samples()};
        auto scc = slice_PCMSamples<int16_t>(container, 1);
        EXPECT_EQ(1, scc.size());

        SlicedChannelContainer<int16_t> test = {
            { testfile_left_samples(), testfile_right_samples()}
        };

        EXPECT_EQ(scc, test);
    }

    TEST_F(ConverterTest, combineTest) {

        ChannelContainer<int16_t> cc{
            {1, 2, 3, INT16_MAX, 5, 7},
            {5, 6, INT16_MIN, 8, 9, 8}};

        SlicedChannelContainer<int16_t> scc{
            {
                {1, 2},
                {5, 6}
            },
            {
                {3, INT16_MAX, 5, 7},
                {INT16_MIN, 8, 9, 8}
            }};

        ChannelContainer<int16_t> tcc = combine_PCMSamples<int16_t>(scc);
        EXPECT_EQ(cc, tcc);
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_ManySamples_B2) {
        ChannelContainer<int16_t> container{ testfile_left_samples(), testfile_right_samples()};
        const int numberChannels = container.size();
        const int buckets = 2;
        auto scc = slice_PCMSamples<int16_t>(container, buckets);
        EXPECT_EQ(buckets, scc.size());

        ChannelContainer<int16_t> buffer = combine_PCMSamples<int16_t>(scc);

        EXPECT_EQ(buffer, container);
    }

    TEST_F(ConverterTest, call_slice_PCMSamples_ManySamples_B20) {
        ChannelContainer<int16_t> container{ testfile_left_samples(), testfile_right_samples()};
        ChannelContainer<int16_t> buffer;
        SlicedChannelContainer<int16_t> scc;

        for (uint8_t b = 0; b < 20; b++) {
            scc = slice_PCMSamples<int16_t>(container, b);
            buffer = combine_PCMSamples<int16_t>(scc);
        }

        EXPECT_EQ(buffer, container);
    }
    
    TEST_F(ConverterTest, call_slice_PCMSamples_ManySamples_B30) {
        ChannelContainer<int16_t> container{ testfile_left_samples(), testfile_right_samples()};
        ChannelContainer<int16_t> buffer;
        SlicedChannelContainer<int16_t> scc;

        for (uint8_t b = 0; b < 30; b++) {
            scc = slice_PCMSamples<int16_t>(container, b);
            buffer = combine_PCMSamples<int16_t>(scc);
        }

        EXPECT_EQ(buffer, container);
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int16) {
        auto data = testfile_WAVE_FORMAT();
        auto b = testfile_Buffer();
        b.erase(b.begin(), b.begin() + 44);

        ChannelContainer<int16_t> samples = get_pcm_Samples<int16_t>(b,
                data.data.dataSize / data.fmt.blockAlignment,
                data.fmt.channels);

        ChannelContainer<int16_t> sc;
        sc.push_back(testfile_left_samples());
        sc.push_back(testfile_right_samples());

        EXPECT_EQ(samples, sc);
    }

    TEST_F(ConverterTest, call_get_pcm_Samples_int16_error) {
        auto data = testfile_WAVE_FORMAT();
        auto b = testfile_Buffer();
        b.erase(b.begin(), b.begin() + 44);

        try {
            ChannelContainer<int16_t> samples = get_pcm_Samples<int16_t>(b,
                    data.data.dataSize / data.fmt.blockAlignment + 1,
                    data.fmt.channels);
            FAIL();
        } catch (ExceptionConverter exc) {
            EXPECT_EQ(exc.code(), BUFFER_SIZE_TOSMALL_FOR_NUMBEROFSAMPLES);
        }
    }

}