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
 * File:   ReadFile.cpp
 * Author: Philipp Kaaden
 * 
 * Created on 20. November 2020, 07:24
 */

#include <algorithm>

#include "ReadFile.h"

namespace encoder {

    ExceptionReadFile::ExceptionReadFile(const CodeExceptionReadFile msg) :
    errmsg(msg) {

    }

    const CodeExceptionReadFile ExceptionReadFile::code() {
        return errmsg;
    }

    ReadFile::ReadFile(const std::string& filepath) :
    filename(filepath),
    ifs(filepath, std::ios::binary) {
        if (!ifs) {
            throw ExceptionReadFile(OPEN_READFILE_ERROR);
        }
    }

    ReadFile::~ReadFile() {
    }

    const std::vector<uint8_t>
    ReadFile::next(const std::size_t number) {
        std::vector<uint8_t> buffer;
        buffer.reserve(number);
        auto c = ifs.peek();
        for (std::size_t n = 0; n < number && c != EOF; n++) {
            c = ifs.get();
            buffer.push_back(c);
            c = ifs.peek();
        }
        return buffer;
    }

    const std::vector<uint8_t>
    ReadFile::peek(const std::size_t number) {
        std::ifstream pifs{filename, std::ios::binary};
        std::vector<uint8_t> buffer;
        buffer.reserve(number);
        auto c = pifs.peek();
        for (std::size_t n = 0; n < number && c != EOF; n++) {
            c = pifs.get();
            buffer.push_back(c);
            c = pifs.peek();
        }
        return buffer;
    }

    const std::vector<uint8_t> ReadFile::entire() {
        std::ifstream pifs{filename, std::ios::binary};
        std::vector<uint8_t> buffer;
        auto c = pifs.peek();
        while (c != EOF) {
            c = pifs.get();
            buffer.push_back(c);
            c = pifs.peek();
        }
        return buffer;
    }
}