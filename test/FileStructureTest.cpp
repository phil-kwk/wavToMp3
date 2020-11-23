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
        const char * folder = "/mnt/c/wavToMp3encoder/testfiles/";
        const char * file = "/mnt/c/wavToMp3encoder/testfiles/testcase.wav";

        const char * sky = "/mnt/c/wavToMp3encoder/testfiles/SkyWorld_sint16_pcm.wav";
    }

    class FileStructureTest : public testing::Test {
    protected:
    };

    TEST_F(FileStructureTest, getfilesInFolder) {
        DIR * dirp;
        struct dirent *dp;

        if ((dirp = opendir(folder)) == NULL) {
            FAIL();
        }

        do {
            if ((dp = readdir(dirp)) != NULL) {
                //(void) printf("found %s\n", dp->d_name);
            }
        } while (dp != NULL);
        closedir(dirp);
    }

    TEST_F(FileStructureTest, filterfilesInFolder) {
        DIR * dirp;
        struct dirent *dp;

        if ((dirp = opendir(folder)) == NULL) {
            FAIL();
        }

        do {
            if ((dp = readdir(dirp)) != NULL) {
                if (strncmp(strrchr(dp->d_name, '.'), ".wav", 4) == 0) {
                    //(void) printf("found %s\n", dp->d_name);
                }
            }
        } while (dp != NULL);
        closedir(dirp);
    }

    TEST_F(FileStructureTest, folderexists) {
        EXPECT_TRUE(directoryExists(folder));
    }

    TEST_F(FileStructureTest, doesNotExists) {
        EXPECT_FALSE(directoryExists("hahahaha"));
    }

    TEST_F(FileStructureTest, doesFileExists) {
        
        EXPECT_FALSE(directoryExists(file));
    }

    TEST_F(FileStructureTest, FolderContainsFiles) {
        

        auto files = getFilesFrom(folder);
        EXPECT_GT(files.size(), 1);

        for (auto f : files) {
            //std::cout << f << "\n";
        }
    }

    TEST_F(FileStructureTest, FileContainsFiles) {
        

        try {
            getFilesFrom(file);
            FAIL();
        } catch (FileSystemError err) {
            SUCCEED() << err.msg();
        }
        
    }

    TEST_F(FileStructureTest, WAVFilterFiles) {
        
        try {
            auto files = getFilesFrom(folder);
            std::vector<std::string> filteredFiles;

            std::copy_if(files.begin(), files.end(), std::back_inserter(filteredFiles),
                    [](std::string item) {
                        return ((strncmp(strrchr(item.c_str(), '.'), ".wav", 4) == 0));
                    });

            for (auto f : files) {
                //std::cout << f << "\n";
            }
            //std::cout << "Filered Files" << "\n";        
            for (auto f : filteredFiles) {
                //std::cout << f << "\n";
            }

        } catch (FileSystemError err) {
            FAIL() << err.msg();
        }
    }

    TEST_F(FileStructureTest, getFilteredFiles) {
        
        try {
            auto files = getFileTypeFrom(folder, "wav");

            //std::cout << "Filered Files" << "\n";
            for (auto f : files) {
                //std::cout << f << "\n";
            }

        } catch (FileSystemError err) {
            FAIL() << err.msg();
        }
    }
}