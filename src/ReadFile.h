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
 * File:   ReadFile.h
 * Author: Philipp Kaaden
 *
 * Created on 20. November 2020, 07:24
 */

#ifndef READFILE_H
#define READFILE_H

#include <fstream>
#include <vector>
#include <cstdint>
#include <exception>

namespace encoder {

    enum CodeExceptionReadFile {
        OPEN_READFILE_ERROR = 10
    };

    class ExceptionReadFile : std::exception {
    public:
        ExceptionReadFile(const CodeExceptionReadFile msg);
        const CodeExceptionReadFile code();
    private:
        const CodeExceptionReadFile errmsg;
    };

    class ReadFile {
    public:
        /**
         * @throws ExceptionReadFile(OPEN_FILE_ERROR) if file could not be opened
         * @param filepath
         */
        ReadFile(const std::string& filepath);
        virtual ~ReadFile();

        /**
         * Read next number Elements from File read postion is advaced by Elements read
         * @param number of Elements to read from File
         * @return std::vector<uint8_t> with number elements read from File (if smaller than number then EOF reached)
         */
        const std::vector<uint8_t> next(const std::size_t number);

        /**
         * always return first number elements in file
         * @param elements
         * @return 
         */
        const std::vector<uint8_t> peek(const std::size_t number);
        
        /**
         * read entire file to buffer
         */
        const std::vector<uint8_t> entire();
    private:
        const std::string filename;
        std::ifstream ifs;
    };

}//end namespace
#endif /* READFILE_H */

