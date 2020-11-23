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

    TEST_F(FileStructureTest, getFilteredFiles) {

        try {
            auto files = getFileTypeFrom(folder, "wav");

        } catch (FileSystemError err) {
            FAIL() << err.msg();
        }
    }
}