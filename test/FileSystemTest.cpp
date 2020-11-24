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

namespace encoder {

    namespace {
        const char * folder = "../";
    }

    class FileStructureTest : public testing::Test {
    protected:
    };

    TEST_F(FileStructureTest, folderexists) {
        EXPECT_TRUE(directoryExists(folder));
    }

    TEST_F(FileStructureTest, doesNotExists) {
        EXPECT_FALSE(directoryExists("hahahaha"));
    }

    TEST_F(FileStructureTest, FolderContainsFiles) {
        auto files = getFilesFrom(folder);
        EXPECT_GT(files.size(), 1);
    }

    TEST_F(FileStructureTest, getFilesFrom_Nonexisting_Folder) {
        try {
            auto files = getFilesFrom("doesNotExist128712");
        } catch (ExceptionFileSystem exc) {
            EXPECT_EQ(exc.code(), OPEN_DIRECTORY_FAILED);
        }
    }

    TEST_F(FileStructureTest, getFilesFrom_Existing_Folder) {
        auto files = getFilesFrom(folder);
    }

    TEST_F(FileStructureTest, getFilteredFiles_Existing_Folder) {

        try {
            auto files = getFileTypeFrom(folder, "txt");
            EXPECT_EQ(files.size(), 2);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }

    TEST_F(FileStructureTest, getFilteredFiles_Existing_Folder_withDot) {

        try {
            auto files = getFileTypeFrom(".", ".sh");
            EXPECT_EQ(files.size(), 3);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }

    TEST_F(FileStructureTest, getFilteredFiles_Existing_Folder_noDot) {

        try {
            auto files = getFileTypeFrom(".", "sh");
            EXPECT_EQ(files.size(), 3);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }
    
    TEST_F(FileStructureTest, getFilteredFiles_Existing_Folder_gitignore) {

        try {
            auto files = getFileTypeFrom("../", ".gitignore");
            EXPECT_EQ(files.size(), 1);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }
    
    TEST_F(FileStructureTest, getFilteredFiles_Existing_Folder_empty) {

        try {
            auto files = getFileTypeFrom("../", "");//searches for all files with dot
            EXPECT_GT(files.size(), 1);
        } catch (ExceptionFileSystem err) {
            FAIL() << "ExceptionFileSystem " << err.code();
        }
    }
    
    TEST_F(FileStructureTest, getFilteredFiles_NonExisting_Folder) {

        try {
            auto files = getFileTypeFrom("notExistingFolder4245", "txt");
            FAIL();
        } catch (ExceptionFileSystem err) {
            EXPECT_EQ( err.code(), OPEN_DIRECTORY_FAILED);
        }
    }

}