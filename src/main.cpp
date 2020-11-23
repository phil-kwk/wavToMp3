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
 * File:   main.cpp
 * Author: Philipp Kaaden
 *
 * Created on 11. November 2020, 13:41
 */

#include <vector>
#include <thread>
#include <cstdint>

#include "ArgumentParser.h"
#include "FileEncodingTask.h"

/*
 * 
 */
int main(int argc, char* argv[]) {


    auto files = encoder::getFilesToEncode(argc, argv);

    const std::size_t concurrentlyEncoding = 2;

    for (auto filename : files) {
        std::thread t(encoder::startFileEncoding, filename, 10000000, 4);
        t.join();
    }

    return 0;
}

