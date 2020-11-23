/* 
 * File:   EncodingTask
 * Author: Philipp Kaaden
 *
 * Created on 14.11.2020, 15:01:17
 */

#include <gtest/gtest.h>

#include <thread>

#include "../src/FileEncodingTask.h"

namespace encoder {

    namespace {
        const std::string testfile{"/mnt/c/wavToMp3encoder/testfiles/testcase.wav"};
        const std::string testfilecountUp30s{"/mnt/c/wavToMp3encoder/testfiles/countUp30.wav"};
        const std::string testfileMusik{"/mnt/c/wavToMp3encoder/testfiles/SkyWorld_sint16_pcm.wav"};

    }

    class FileEncodingTaskTest : public testing::Test {
    protected:
    };

    TEST_F(FileEncodingTaskTest, start) {
        startFileEncoding(testfile);
    }

    TEST_F(FileEncodingTaskTest, constructor_fail) {
        try {
            startFileEncoding("HAHA");
            FAIL();
        } catch (...) {

        }
    }
    
    TEST_F(FileEncodingTaskTest, encodetestfilecountUp30s) {
        std::thread t(startFileEncoding,testfilecountUp30s,5000000,1);
        
        t.join();
    }
    
}//end namespace