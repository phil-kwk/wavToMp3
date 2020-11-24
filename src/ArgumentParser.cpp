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

#include "ArgumentParser.h"
#include "FileSystem.h"

namespace encoder {

    namespace {

        const std::vector<std::string>
        getArguments(int argc, char* argv[]) {

            std::vector<std::string> arguments;
            for (int n = 0; n < argc; n++) {
                arguments.push_back(argv[n]);
            }
            return arguments;
        }

        void showHelp() {
            std::cout << "usage: wavToMp3 [directoryPath]" << "\n";
        }

        void showWelcome() {
            std::cout << "Started wavToMp3 " << "\n"
                    << "searching for files to encode in " << "\n";
        }
    }

    const std::vector< std::string >
    getFilesToEncode(int argc, char* argv[]) {

        if (argc < 2) {
            showHelp();
            return {};
        }
        
        showWelcome();

        auto args = getArguments(argc, argv);

        std::vector< std::string > wavfiles;
        for (auto a : args) {
            std::cout << a << "\n";
            if (directoryExists(a)) {
                auto files = getFileTypeFrom(a, "wav");
                wavfiles.insert(wavfiles.begin(), files.begin(), files.end());
            }
        }

        return wavfiles;
    }

}