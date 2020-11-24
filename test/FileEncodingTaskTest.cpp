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
        
    }

    class FileEncodingTaskTest : public testing::Test {
    protected:

        void SetUp() {
            WriteFile file(fileName);
            file.write(testfile_Buffer());
        }

    };

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
    
    TEST_F(FileEncodingTaskTest, startFileEncoding_CorruptFile_dataLengthToBig) {
        const std::string corruptFileName = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        std::vector<uint8_t> cB = testRiff_Buffer();
        cB[40] = 200;//dataLength to big
        WriteFile file(corruptFileName);
        file.write(cB);
        
        startFileEncoding(corruptFileName);
    }
    
    TEST_F(FileEncodingTaskTest, startFileEncoding_CorruptFile_dataAlignmentWrong) {
        const std::string corruptFileName = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        std::vector<uint8_t> cB = testRiff_Buffer();
        cB[40] = 201;//not aligned with BlockAlignment
        WriteFile file(corruptFileName);
        file.write(cB);
        
        startFileEncoding(corruptFileName);
    }

    TEST_F(FileEncodingTaskTest, startFileEncoding_CorruptFile_UnevenFileLength1) {
        const std::string corruptFileName = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        std::vector<uint8_t> cB = testRiff_Buffer();
        cB.push_back(0xaa);
        WriteFile file(corruptFileName);
        file.write(cB);
        
        startFileEncoding(corruptFileName);
    }
    
    TEST_F(FileEncodingTaskTest, startFileEncoding_CorruptFile_UnevenFileLength3) {
        const std::string corruptFileName = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        std::vector<uint8_t> cB = testRiff_Buffer();
        cB.push_back(0xaa);cB.push_back(0xaa);cB.push_back(0xaa);
        WriteFile file(corruptFileName);
        file.write(cB);
        
        startFileEncoding(corruptFileName);
    }
    
    //here a Problem exists since it will emcode everything even after Data until entire fIle has been encoded
    TEST_F(FileEncodingTaskTest, startFileEncoding_CorruptFile_ExtendedFileLength4) {
        const std::string corruptFileName = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        std::vector<uint8_t> cB = testRiff_Buffer();
        cB.push_back(0xaa);cB.push_back(0xaa);cB.push_back(0xaa);cB.push_back(0xaa);
        WriteFile file(corruptFileName);
        file.write(cB);
        
        startFileEncoding(corruptFileName);
    }
    
}//end namespace