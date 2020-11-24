/* 
 * File:   EncoderTest
 * Author: Philipp Kaaden
 *
 * Created on 13.11.2020, 14:50:05
 */

#include <gtest/gtest.h>

#include <chrono>
#include <sstream>
#include <tuple>

#include "../src/Encoder.h"
#include "../src/ReadFile.h"
#include "../src/WriteFile.h"
#include "../src/Parser.h"
#include "../src/SampleConverter.h"

#include "TestWaveData.h"



namespace encoder {
    namespace {
        const std::string testfile{"testcase.wav"};
    }

    class EncoderTest : public testing::Test {
    protected:
        void SetUp(){
            WriteFile file(testfile);
            file.write(testfile_Buffer());
        }
    };

    TEST_F(EncoderTest, TestEncodeSeriell) {
        const WAVE_Format wave{ testfile_WAVE_FORMAT()};
        const ChannelContainer<int16_t> cc{testfile_ChannelContainerSamples()};
        try {
            auto mp3Buffer = wavToMp3Seriell<int16_t>(wave.fmt, cc);

            EXPECT_GT(mp3Buffer.size(), wave.data.dataSize / 15); //approximation

            WriteFile file("EncoderTest.TestEncodeSeriell.mp3");
            file.write(mp3Buffer);
        } catch (ExceptionEncoder err) {
            FAIL() << err.code();
        }
    }

    const std::size_t getSampleSize(const WAVE_Format& wave) {
        return wave.data.dataSize / wave.fmt.blockAlignment;
    }

    std::tuple< const ChannelContainer<int16_t>, const WAVE_Format>
    getContainer(const std::string& filename) {
        ReadFile read(filename);
        auto buffer = read.entire();

        const auto waveFormat = get_WaveFormat(buffer);

        moveBufferToSamplePosition(buffer);

        const std::size_t sampleSize = getSampleSize(waveFormat);
        const std::size_t numberChannels = waveFormat.fmt.channels;

        return {get_pcm_Samples<int16_t>(buffer, sampleSize, numberChannels), waveFormat};
    }

    TEST_F(EncoderTest, readParseEncodeWrite_seriell) {
        const std::string filename{testfile};
        try {
            auto start = std::chrono::high_resolution_clock::now();
            auto result = getContainer(filename);
            auto container = std::get<0>(result);
            auto waveFormat = std::get<1>(result);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                    << " ms read and Parse file" << "\n";

            start = std::chrono::high_resolution_clock::now();
            auto mp3Buffer = wavToMp3Seriell<int16_t>(waveFormat.fmt, container);
            end = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                    << " ms encoding" << "\n";

            EXPECT_GT(mp3Buffer.size(), waveFormat.data.dataSize / 15); //approximation

            start = std::chrono::high_resolution_clock::now();
            WriteFile file("TestSuite.readParseEncodeWrite_seriell.mp3");
            file.write(mp3Buffer);
            end = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                    << " ms write File" << "\n";
        } catch (ExceptionEncoder err) {
            FAIL() << err.code();
        }
    }

    TEST_F(EncoderTest, readParseEncodeWrite_concurently) {
        const std::string filename{testfile};
        try {
            auto start = std::chrono::high_resolution_clock::now();
            auto result = getContainer(filename);
            auto container = std::get<0>(result);
            auto waveFormat = std::get<1>(result);
            auto end = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                    << " ms read and Parse file" << "\n";

            start = std::chrono::high_resolution_clock::now();
            auto mp3Buffer = wavToMp3Concurrently<int16_t>(waveFormat.fmt, container, 4);
            end = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                    << " ms encoding" << "\n";

            EXPECT_GT(mp3Buffer.size(), waveFormat.data.dataSize / 15); //approximation

            start = std::chrono::high_resolution_clock::now();
            WriteFile file("TestSuite.readParseEncodeWrite_concurently.mp3");
            file.write(mp3Buffer);
            end = std::chrono::high_resolution_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                    << " ms write File" << "\n";
        } catch (ExceptionEncoder err) {
            FAIL() << err.code();
        }
    }
}//end namespace