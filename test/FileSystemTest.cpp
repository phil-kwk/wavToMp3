/* 
 * File:   FileStructureTest
 * Author: Philipp Kaaden
 *
 * Created on 13.11.2020, 16:31:25
 */

#include <gtest/gtest.h>

#include <dirent.h>
#include <algorithm>
#include <complex>

#include "../src/FileSystem.h"
#include "../src/WriteFile.h"

namespace encoder {

    namespace {
        const char * folderNotExist = "bwehn23895yhgfn";
        const char * folder = "../";
        const std::string testfile = "testfile.txt";
    }

    class FileSystemTest : public testing::Test {
    protected:
        void SetUp(){
            WriteFile file(testfile);
            file.write(std::vector<uint8_t>(1000,0xaf));
        }

    };

    TEST_F(FileSystemTest, directoryExists_ok) {
        EXPECT_TRUE(directoryExists(folder));
    }

    TEST_F(FileSystemTest, directoryExists_not) {
        EXPECT_FALSE(directoryExists(folderNotExist));
    }

    TEST_F(FileSystemTest, FolderContainsFiles) {
        auto files = getFilesFrom(folder);
        EXPECT_GT(files.size(), 1);
    }

    TEST_F(FileSystemTest, getFilesFrom_Nonexisting_Folder) {
        try {
            auto files = getFilesFrom(folderNotExist);
        } catch (ExceptionFileSystem exc) {
            EXPECT_EQ(exc.code(), OPEN_DIRECTORY_FAILED);
        }
    }

    TEST_F(FileSystemTest, getFileTypeFrom_Existing_Folder) {

        try {
            auto files = getFileTypeFrom(folder, "txt");
            EXPECT_GE(files.size(), 1);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }

    TEST_F(FileSystemTest, getFilteredFiles_Existing_Folder_withDot) {

        try {
            auto files = getFileTypeFrom(".", ".sh");
            EXPECT_GE(files.size(), 4);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }

    TEST_F(FileSystemTest, getFilteredFiles_Existing_Folder_noDot) {

        try {
            auto files = getFileTypeFrom(".", "sh");
            EXPECT_GE(files.size(), 4);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }
    
//    TEST_F(FileSystemTest, getFilteredFiles_Existing_Folder_gitignore) {
//
//        try {
//            auto files = getFileTypeFrom("../", ".gitignore");
//            EXPECT_EQ(files.size(), 1);
//        } catch (ExceptionFileSystem err) {
//            FAIL() << "ExceptionFileSystem " << err.code();
//        }
//    }
    
    TEST_F(FileSystemTest, getFilteredFiles_Existing_Folder_empty) {

        try {
            auto files = getFileTypeFrom(folder, "");//searches for all files with dot
            EXPECT_GT(files.size(), 1);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }
    
    TEST_F(FileSystemTest, getFilteredFiles_NonExisting_Folder) {

        try {
            auto files = getFileTypeFrom(folderNotExist, "txt");
            FAIL();
        } catch (ExceptionFileSystem err) {
            EXPECT_EQ( err.code(), OPEN_DIRECTORY_FAILED);
        }
    }

}