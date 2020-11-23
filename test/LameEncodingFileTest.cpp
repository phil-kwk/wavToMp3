/* 
 * File:   LameEncodingFileTest
 * Author: Philipp Kaaden
 *
 * Created on 17.11.2020, 10:03:44
 */

#include <gtest/gtest.h>

#include "../src/LameEncoder.h"
#include "../src/WriteFile.h"

#include "TestWaveData.h"

namespace encoder {

    class LameEncodingFileTest : public testing::Test {
    protected:
    };

    TEST_F(LameEncodingFileTest, encodeWrite) {
        const WAVE_Format wave {testfile_WAVE_FORMAT()};
        const ChannelContainer<int16_t> cc {testfile_ChannelContainerSamples()};
        
        try {
            LameEncoder<int16_t> encoder;
            encoder.setConfig(wave.fmt, good);
            auto mp3 = encoder.encodePCMSamples(cc);
            auto flush = encoder.encode_flush(false);
            mp3.insert(mp3.end(), flush.begin(), flush.end());
            
            WriteFile file("LameEncodingFileTest.encodeWrite.mp3");
            file.write(mp3);
        } catch (ExceptionLameEncoding err) {
            FAIL() << "ExceptionLameEncoding " << err.msg();
        }catch (ExceptionWriteFile err) {
            FAIL() << "ExceptionWriteFile " << err.code();
        }
    }

}//end encoder