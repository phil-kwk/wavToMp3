/* 
 * File:   EncodingTask
 * Author: Philipp Kaaden
 *
 * Created on 14.11.2020, 15:01:17
 */

#include <gtest/gtest.h>

#include <thread>

#include "../src/FileEncodingTask.h"
#include "../src/WriteFile.h"

#include "TestWaveData.h"

namespace encoder {

    namespace {
        const std::string fileName = "FileEncodingTaskTest.wav";
        const std::string corruptFileName = "corrupt.wav";
    }

    class FileEncodingTaskTest : public testing::Test {
    protected:

        void SetUp() {
            WriteFile file(fileName);
            file.write(testfile_Buffer());
        }

    };

    void writeCorruptFile() {
        std::vector<uint8_t> corruptBuffer;

        WriteFile file(corruptFileName);
        file.write(testfile_Buffer());
    }

    TEST_F(FileEncodingTaskTest, startFileEncoding_DefaultParams) {
        startFileEncoding(fileName);
    }

    TEST_F(FileEncodingTaskTest, startFileEncoding_50kB_T1) {
        startFileEncoding(fileName, 50000, 1);
    }

    TEST_F(FileEncodingTaskTest, startFileEncoding_5kB_T1) {
        startFileEncoding(fileName, 5000, 1); //settings should not be used for encoding
    }

    TEST_F(FileEncodingTaskTest, startFileEncoding_5kB_T5) {
        startFileEncoding(fileName, 5000, 5); //settings should not be used for encoding
    }

    TEST_F(FileEncodingTaskTest, startFileEncoding_NotExistingFileName) {
        startFileEncoding("t5wcnlawh9cval.wav");
    }
    
    TEST_F(FileEncodingTaskTest, startFileEncoding_CorruptFile) {
        startFileEncoding(corruptFileName);
    }

}//end namespace