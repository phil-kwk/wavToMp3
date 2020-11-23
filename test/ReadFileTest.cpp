/* 
 * File:   ReadFileTest
 * Author: Philipp Kaaden
 *
 * Created on 20.11.2020, 07:24:58
 */

#include <string>
#include <iostream>
#include <algorithm>

#include <gtest/gtest.h>
#include "TestWaveData.h"

#include "../src/ReadFile.h"

namespace encoder {

    namespace {
        const std::string pathname = "/mnt/c/wavToMp3encoder/testfiles/testcase.wav";
        const std::string dirname = "/mnt/c/wavToMp3encoder/testfiles";
    }

    class ReadFileTest : public testing::Test {
    protected:
    };

    TEST_F(ReadFileTest, constructor_ok) {
        try {
            ReadFile read{pathname};
        } catch (ExceptionReadFile exc) {
            FAIL();
        }
    }

    TEST_F(ReadFileTest, constructor_Exception) {
        try {
            ReadFile read{""};
            FAIL();
        } catch (ExceptionReadFile exc) {
            EXPECT_EQ(exc.code(), OPEN_READFILE_ERROR);
        }
    }

    TEST_F(ReadFileTest, constructor_Exception_Dir) {
        try {
            ReadFile read{dirname};
            //            FAIL() << "That was not expected";TODO
        } catch (ExceptionReadFile exc) {
            EXPECT_EQ(exc.code(), OPEN_READFILE_ERROR);
        }
    }

    TEST_F(ReadFileTest, get_NextElements) {
        const int number = 9999;
        try {
            ReadFile read{pathname};
            auto buffer = read.next(number);
            EXPECT_EQ(buffer.size(), number);
        } catch (ExceptionReadFile exc) {
            FAIL();
        }
    }

    TEST_F(ReadFileTest, get_NextElements_TestFileBuffer) {
        const int number = testfile_Buffer().size();
        try {
            ReadFile read{pathname};
            auto buffer = read.next(number);
            EXPECT_EQ(buffer.size(), number);
            EXPECT_EQ(buffer, testfile_Buffer());
        } catch (ExceptionReadFile exc) {
            FAIL();
        }
    }

    TEST_F(ReadFileTest, get_NextElementsInParts) {
        const int number = 1152;
        try {
            ReadFile read{pathname};
            std::vector<uint8_t> buffer;

            for (int n = 0; n < 90; n++) {
                auto b = read.next(number);
                buffer.insert(buffer.end(),
                        b.begin(),
                        b.end());
            }
            EXPECT_EQ(buffer.size(), testfile_Buffer().size());
            EXPECT_EQ(buffer, testfile_Buffer());
        } catch (ExceptionReadFile exc) {
            FAIL();
        }
    }

    TEST_F(ReadFileTest, call_peek_Number) {
        const int number = 10;
        try {
            ReadFile read{pathname};
            auto buffer = read.peek(number);
            EXPECT_EQ(buffer.size(), number);
            auto b2 = testfile_Buffer();
            b2.resize(number);
            EXPECT_EQ(buffer.size(), number);
            EXPECT_EQ(buffer, b2);
        } catch (ExceptionReadFile exc) {
            FAIL();
        }
    }

    TEST_F(ReadFileTest, call_peek_Repeatetly) {
        const int number = 1152;
        try {
            std::vector<uint8_t> buffer;
            ReadFile read{pathname};
            auto b2 = testfile_Buffer();
            b2.resize(number);
            
            for (int i = 0; i < 5; i++) {
                buffer = read.peek(number);
                EXPECT_EQ(buffer.size(), number);
                EXPECT_EQ(buffer, b2);
            }
        } catch (ExceptionReadFile exc) {
            FAIL();
        }
    }
    
    TEST_F(ReadFileTest, call_next_peak_Together) {
        const int number = 1152;
        try {
            std::vector<uint8_t> buffer;
            ReadFile read{pathname};
            
            auto b2 = testfile_Buffer();
            b2.resize(number);
            
            read.next(number);
            
            for (int i = 0; i < 5; i++) {
                buffer = read.peek(number);
                EXPECT_EQ(buffer.size(), number);
                EXPECT_EQ(buffer, b2);
            }
        } catch (ExceptionReadFile exc) {
            FAIL();
        }
    }

    TEST_F(ReadFileTest, readEntireFile) {
        try {
            ReadFile read{pathname};
            auto buffer = read.entire();
            EXPECT_EQ(buffer.size(), testfile_Buffer().size());
            EXPECT_EQ(buffer, testfile_Buffer());
        } catch (ExceptionReadFile exc) {
            FAIL();
        }
    }
    
}