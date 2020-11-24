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
 * File:   ArgumentParser.h
 * Author: Philipp Kaaden
 *
 * Created on 23. November 2020, 10:33
 */

#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <vector>
#include <iostream>

namespace encoder {
    
    /**
     * returns a vector with the Filenames that should be encoded
     * @param argc
     * @param argv
     * @return 
     */
    const std::vector< std::string > getFilesToEncode(int argc, char* argv[]);

}//end namespace

#endif /* ARGUMENTPARSER_H */

