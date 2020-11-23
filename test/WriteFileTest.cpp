/* 
 * File:   WriteFileTest
 * Author: Philipp Kaaden
 *
 * Created on 20.11.2020, 09:24:40
 */

#include <gtest/gtest.h>

#include "../src/WriteFile.h"
#include "../src/ReadFile.h"

#include "TestWaveData.h"

namespace encoder {

    class WriteFileTest : public testing::Test {
    protected:
    };

    TEST_F(WriteFileTest, openFileTest) {
        WriteFile file("openFileTest");
    }

    TEST_F(WriteFileTest, openFileTestTwice) {
        WriteFile file1("openFileTestTwice");
        WriteFile file2("openFileTestTwice");
    }

    TEST_F(WriteFileTest, writeToFile) {
        auto name = "writeToFile";
        std::vector<uint8_t> wbuffer = {'h', 'a', 'l', 'l', 'o'};


        WriteFile file(name);
        file.write(wbuffer);

        ReadFile read(name);
        auto rbuffer = read.entire();

        EXPECT_EQ(wbuffer, rbuffer);
    }

    TEST_F(WriteFileTest, writeToFile_Repeatedly) {
        auto name = "writeToFile_Repeatedly";
        std::vector<uint8_t> wbuffer = {'h', 'a', 'l', 'l', 'o'};

        WriteFile file(name);
        for (int i = 0; i < 10; i++) {
            file.write(wbuffer);
        }
        
        ReadFile read(name);
        auto rbuffer = read.entire();

        EXPECT_EQ(wbuffer.size() * 10, rbuffer.size());
    }

    TEST_F(WriteFileTest, writeToFile_BigFile) {
        auto name = "writeToFile_BigFile";
        std::vector<uint8_t> wbuffer = testfile_Buffer();

        WriteFile file(name);
        file.write(wbuffer);

        
        ReadFile read(name);
        auto rbuffer = read.entire();

        EXPECT_EQ(wbuffer, rbuffer);
        EXPECT_EQ(wbuffer.size(), rbuffer.size());
    }
    
}