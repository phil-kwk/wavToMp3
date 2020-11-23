/*
 * Copyright (C) 2020 Philipp Kaaden.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

/* 
 * File:   FileSystem.cpp
 * Author: Philipp Kaaden
 * 
 * Created on 14. November 2020, 08:07
 */

#include "FileSystem.h"

#include  <string.h>
#include <dirent.h>
#include <ftw.h>

#include <sstream>
#include <fstream>
#include <algorithm>

namespace encoder {

    FileSystemError::FileSystemError(const std::string& msg) : errmsg(msg) {

    }

    const std::string FileSystemError::msg() {
        return errmsg;
    }

    const bool directoryExists(const std::string& folder) {

        DIR * dirp = opendir(folder.c_str());
        if (dirp == NULL) {
            return false;
        }
        if (closedir(dirp) < 0) {
            throw FileSystemError("closedir failed");
        }
        return true;
    }

    const std::vector<std::string> getFilesFrom(
            const std::string& folder) {

        DIR * dirp = opendir(folder.c_str());
        if (dirp == NULL) {
            std::stringstream msg;
            msg << "opendir failed" << folder;
            throw FileSystemError(msg.str());
        }

        std::vector<std::string> files;
        struct dirent *dp;
        do {
            if ((dp = readdir(dirp)) != NULL) {

                std::stringstream file;
                if (folder.back() == '/') {
                    file << folder << dp->d_name;
                } else {
                    file << folder << "/" << dp->d_name;
                }
                if (!directoryExists(file.str())) {
                    files.push_back(file.str());
                }
            }
        } while (dp != NULL);
        if (closedir(dirp) < 0) {
            throw FileSystemError("closedir failed");
        }
        return files;
    }

    const std::vector<std::string> getFileTypeFrom(
            const std::string& folder,
            const char* fileExtension) {

        auto files = getFilesFrom(folder);
        std::vector<std::string> filteredFiles;
        std::copy_if(files.begin(), files.end(), std::back_inserter(filteredFiles),
                [fileExtension](std::string item) {
                    return ((strncmp(strrchr(item.c_str(), '.') + 1, fileExtension, 4) == 0));
                });
        return filteredFiles;
    }

}//end namespace