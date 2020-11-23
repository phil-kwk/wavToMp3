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

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>


#include "Parser.h"
#include "Encoder.h"
#include "FileSystem.h"
#include "ReadFile.h"

const std::vector<std::string>
getArguments(int argc, char* argv[]) {

    std::vector<std::string> arguments;
    for (int n = 0; n < argc; n++) {
        arguments.push_back(argv[n]);
    }
    return arguments;
}

void showHelp() {
    std::cout << "wavToMp3 -h #show this output" << "\n"
            << "wavToMp3 /path/To/Folder" << "\n"
            << "#will attemt to convert all *.wav Files in the Folder to *.mp3 Files" << "\n";
}

const std::string filepath{"/mnt/c/wavToMp3encoder/testfiles/SkyWorld_sint16_pcm.wav"};

void read_parse_encode_write(const std::string filename) {
    try {
        encoder::ReadFile file{filename};
        auto buffer = file.entire();


//        auto mp3 = wavToMp3(wave.fmt, wave.data.samples);

//        std::stringstream outName;
//        outName << filename << ".mp3";
//        file.writeFile(outName.str(), mp3);
    } catch (...) {
        std::cout << "error";
    }
}

/*
 * 
 */
int main(int argc, char* argv[]) {
    std::cout << "wavToMp3 Started" << "\n";

    read_parse_encode_write(filepath);

    return 0;
}

