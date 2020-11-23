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
 * File:   WriteFile.cpp
 * Author: Philipp Kaaden
 * 
 * Created on 20. November 2020, 09:24
 */

#include "WriteFile.h"

namespace encoder {
    ExceptionWriteFile::ExceptionWriteFile(const CodeExceptionWriteFile msg):
    errmsg(msg){

    }

    const CodeExceptionWriteFile ExceptionWriteFile::code() {
        return errmsg;
    }

    
    WriteFile::WriteFile(const std::string& filepath) :
    ofs(filepath, std::ios::binary) {
        if (!ofs) {
            throw ExceptionWriteFile(OPEN_WRITEFILE_ERROR);
        }
    }

    WriteFile::~WriteFile() {
    }

    void WriteFile::write(const std::vector<uint8_t>& buffer) {
        for (uint8_t c : buffer) {
            ofs.rdbuf()->sputc(c);
        }
        ofs.flush();
    }


}//end namespace