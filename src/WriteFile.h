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
 * File:   WriteFile.h
 * Author: Philipp Kaaden
 *
 * Created on 20. November 2020, 09:24
 */

#ifndef WRITEFILE_H
#define WRITEFILE_H

#include <fstream>
#include <vector>
#include <exception>
#include <cstdint>

namespace encoder {

    enum CodeExceptionWriteFile {
        OPEN_WRITEFILE_ERROR = 20
    };

    class ExceptionWriteFile : std::exception {
    public:
        ExceptionWriteFile(const CodeExceptionWriteFile msg);
        const CodeExceptionWriteFile code();
    private:
        const CodeExceptionWriteFile errmsg;
    };

    class WriteFile {
    public:
        /**
         * open file for writing if file exists then it will be overwritten
         * @param filepath
         */
        WriteFile(const std::string& filepath);
        virtual ~WriteFile();

        /**
         * adds the buffer content to the open file
         * @param buffer
         */
        void write(const std::vector<uint8_t>& buffer);
    private:
        std::ofstream ofs;
    };


}//end namespace
#endif /* WRITEFILE_H */

