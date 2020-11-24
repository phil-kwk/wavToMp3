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
 * File:   FileSystem.h
 * Author: Philipp Kaaden
 *
 * Created on 14. November 2020, 08:07
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <exception>
#include <vector>
#include <string>

namespace encoder {

    enum CodeExceptionFileSystem{
        OPEN_DIRECTORY_FAILED = 10,
        CLOSE_DIRECTORY_FAILED = 11,
    };
    
    class ExceptionFileSystem : std::exception {
    public:
        ExceptionFileSystem(const CodeExceptionFileSystem& msg);
        const CodeExceptionFileSystem code();
    private:
        const CodeExceptionFileSystem errmsg;
    };

    /**
     * check if given directory path exists
     * @param folder
     * @return true if path is a folder and exists
     */
    const bool directoryExists(const std::string& folder);

    /**
     * get absolute Filenames of all Files in Folder
     * @param folder
     * @return std::vector<std::string> contianing the filnames in the folder
     * @throws ExceptionFileSystem
     */
    const std::vector<std::string> getFilesFrom(
            const std::string& folder);

    /**
     * get absolute Filesnames of all Files with specified file Extension
     * @param folder
     * @param fileExtension
     * @return 
     * @throws ExceptionFileSystem
     */
    const std::vector<std::string> getFileTypeFrom(
            const std::string& folder,
            const std::string& fileExtension);

}//end namespace
#endif /* FILESYSTEM_H */

