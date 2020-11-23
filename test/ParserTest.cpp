/* 
 * File:   WAVFileBufferTest
 * Author: Philipp Kaaden
 *
 * Created on 12.11.2020, 17:51:32
 */

#include <iostream>

#include <string.h>

#include <gtest/gtest.h>

#include "../src/Parser.h"
#include "../src/Formats.h"

#include "TestWaveData.h"

namespace encoder {

    namespace {

        bool compare(const RiffHeader& a, const RiffHeader& b) {
            return (memcmp(b.groupId, a.groupId, sizeof (b.groupId)) == 0) &&
                    b.fileLength == a.fileLength &&
                    (memcmp(b.riftType, a.riftType, sizeof (b.riftType)) == 0);
        }

        bool compare(const WaveFmtHeader& a, const WaveFmtHeader& b) {
            return (memcmp(a.chunkId, b.chunkId, sizeof (a.chunkId)) == 0) &&
                    a.chunkSize == b.chunkSize &&
                    a.formatTag == b.formatTag &&
                    a.channels == b.channels &&
                    a.SamplesPerSec == b.SamplesPerSec &&
                    a.bytesPerSecond == b.bytesPerSecond &&
                    a.blockAlignment == b.blockAlignment &&
                    a.bitsPerSample == b.bitsPerSample;
        }

        bool compare(
                const WaveDataHeader& a,
                const WaveDataHeader& b) {
            return (memcmp(a.chunkId, b.chunkId, sizeof (a.chunkId)) == 0) &&
                    a.dataSize == b.dataSize;
        }

        bool compare(const WAVE_Format& a, const WAVE_Format& b) {
            return compare(a.fmt, b.fmt) && compare(a.data , b.data) && compare(a.riff , b.riff);
        }
    }

    class WAVFormatTest : public testing::Test{
    protected:

    };

    TEST_F(WAVFormatTest, wavParsingtest) {
        try {
            WAVE_Format conf = testRiff_WAVE_FORMAT();
            auto buffer = testRiff_Buffer();
            auto file = get_WaveFormat(buffer);
            EXPECT_TRUE( compare(conf, file) );
        } catch (ParseWAVFileError err) {
            FAIL() << "ParseWAVFileError " << err.code();
        }
    }

    TEST_F(WAVFormatTest, musikParsingtest) {
        try {
            auto buffer = testMusic_Buffer();
            
            auto w = get_WaveFormat(buffer);
            EXPECT_TRUE( compare(testMusic_WAVE_FORMAT(), w ) );
        } catch (ParseWAVFileError err) {
            FAIL() << "ParseWAVFileError " << err.code();
        }
    }

    TEST_F(WAVFormatTest, parse_RIFF_File_Header_emptybuffer) {
        std::vector<uint8_t> buffer;
        try {
            get_RiffHeader(buffer);
            FAIL();
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), BUFFERSIZE_INSUFFICIENT_FOR_RIFF_PARSING);
        }
    }

    TEST_F(WAVFormatTest, parse_RIFF_File_Header_NotRIFF_buffer) {
        std::vector<uint8_t> noRIFF{ testRiff_Buffer()};
        noRIFF[0] = 0x01;
        try {
            get_RiffHeader(noRIFF);
            FAIL();
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), RIFF_ID_CHECK_FAILED);
        }
    }

    TEST_F(WAVFormatTest, parse_RIFF_File_Header_RIFF_Length) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        try {
            auto riff = get_RiffHeader(test);
            EXPECT_EQ(strncmp((const char *) riff.groupId, "RIFF", 4), 0);
            EXPECT_EQ(riff.fileLength, test.size() - 8);
            EXPECT_EQ(strncmp((const char *) riff.riftType, "WAVE", 4), 0);
        } catch (ParseWAVFileError err) {
            FAIL() << err.code();
        }
    }

    //deprected since parsing needs to work even if buffer has not fully loaded
//    TEST_F(WAVFormatTest, parse_RIFF_File_Header_incorrectRIFF_Length) {
//        std::vector<uint8_t> test{ testRiff_Buffer()};
//        test[4] = 0x01;
//        try {
//            get_RiffHeader(test);
//            FAIL();
//        } catch (ParseWAVFileError err) {
//            EXPECT_EQ(err.code(), RIFF_FILE_LENGTH_CHECKFAILED);
//        }
//    }

    TEST_F(WAVFormatTest, parse_RIFF_File_Header_incorrectRiffType) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        test[8] = 0x00;
        try {
            get_RiffHeader(test);
            FAIL();
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), WAVE_ID_CHECK_FAILED);
        }
    }

    TEST_F(WAVFormatTest, parse_FORMAT_Header) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        WAVE_Format conf = testRiff_WAVE_FORMAT();
        try {
            auto fmt = get_WaveFmtHeader(test);
            EXPECT_TRUE( compare(fmt, conf.fmt) );
        } catch (ParseWAVFileError err) {
            FAIL() << err.code();
        }
    }

    TEST_F(WAVFormatTest, parse_FORMAT_Header_notfound) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        test[13] = 0x0;
        try {
            auto fmt = get_WaveFmtHeader(test);
            FAIL();
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), FORMAT_ID_NOT_FOUND);
        }
    }

    TEST_F(WAVFormatTest, parse_FORMAT_bufferSizeToSmall) {
        try {
            auto fmt = get_WaveFmtHeader({});
            FAIL();
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), BUFFERSIZE_INSUFFICIENT_FOR_FORMAT_PARSING);
        }
    }

    TEST_F(WAVFormatTest, parse_FORMAT_chunkSizeError) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        test[16] = 0x0;
        try {
            auto fmt = get_WaveFmtHeader(test);
            FAIL();
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), FORMAT_HEADER_LENGTH_CHECK_FAILED);
        }
    }

    TEST_F(WAVFormatTest, parse_FORMAT_chunkSizeBigger_than_Min) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        test[16] = 20;
        try {
            auto fmt = get_WaveFmtHeader(test);
            EXPECT_EQ(fmt.chunkSize, 20);
        } catch (ParseWAVFileError err) {
            FAIL() << err.code();
        }
    }

    TEST_F(WAVFormatTest, parse_FORMATTAG_WRONG) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        test[20] = 0x00;
        try {
            auto fmt = get_WaveFmtHeader(test);
            FAIL() << fmt.formatTag;
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), WAVE_FORMATTAG_NOT_SUPPORTED);
        }
    }

    TEST_F(WAVFormatTest, parse_DATA_chunkID_notFound) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        test[36] = 'a';
        try {
            auto data = get_WaveDataHeader(test);
            FAIL() << data.chunkId;
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), DATA_ID_NOT_FOUND);
        }
    }

    TEST_F(WAVFormatTest, parse_DATA_buffertosmall) {
        std::vector<uint8_t> test{};
        try {
            auto data = get_WaveDataHeader(test);
            FAIL() << data.dataSize;
        } catch (ParseWAVFileError err) {
            EXPECT_EQ(err.code(), BUFFERSIZE_INSUFFICIENT_FOR_DATA_PARSING);
        }
    }
    
// deprecated since we cannot garantee that hole buffer is loaded at time of parsing
//    TEST_F(WAVFormatTest, parse_DATA_chunkkSizeError) {
//        std::vector<uint8_t> test{ testRiff_Buffer()};
//        test[40] = 0xFF;
//        try {
//            auto data = get_WaveDataHeader(test);
//            FAIL() << data.dataSize;
//        } catch (ParseWAVFileError err) {
//            EXPECT_EQ(err.code(), DATA_LENGTH_BIGGER_THAN_BUFFER);
//        }
//    }
//
//    TEST_F(WAVFormatTest, parse_DATA_chunkkSizeError1) {
//        std::vector<uint8_t> test{ testRiff_Buffer()};
//        test[40] = 0x55;
//        try {
//            auto data = get_WaveDataHeader(test);
//            FAIL() << data.dataSize;
//        } catch (ParseWAVFileError err) {
//            EXPECT_EQ(err.code(), DATA_LENGTH_BIGGER_THAN_BUFFER);
//        }
//    }

    TEST_F(WAVFormatTest, parse_DATA_ok) {
        std::vector<uint8_t> test{ testRiff_Buffer()};
        WAVE_Format conf = testRiff_WAVE_FORMAT();
        try {
            auto data = get_WaveDataHeader(test);
            EXPECT_TRUE( compare(data, conf.data) );
        } catch (ParseWAVFileError err) {
            FAIL() << err.code();
        }
    }
    
    TEST_F(WAVFormatTest, parse_DATA_musik_ok) {
        std::vector<uint8_t> test{ testMusic_Buffer()};
        WAVE_Format conf = testMusic_WAVE_FORMAT();
        try {
            auto data = get_WaveDataHeader(test);
            EXPECT_TRUE( compare(data, conf.data) );
        } catch (ParseWAVFileError err) {
            FAIL() << err.code();
        }
    }
    
    TEST_F(WAVFormatTest, parse_DATA_testFile_ok) {
        std::vector<uint8_t> test{ testfile_Buffer()};
        WAVE_Format conf = testfile_WAVE_FORMAT();
        try {
            auto data = get_WaveDataHeader(test);
            EXPECT_TRUE( compare(data, conf.data) );
        } catch (ParseWAVFileError err) {
            FAIL() << err.code();
        }
    }

}//end namespace