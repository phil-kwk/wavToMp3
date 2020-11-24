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
 * File:   FileEncodingTask.h
 * Author: Philipp Kaaden
 *
 * Created on 22. November 2020, 17:35
 */

#include <thread>
#include <cmath>

#ifndef FILEENCODINGTASK_H
#define FILEENCODINGTASK_H

namespace encoder {
    
    /**
     * @hint start in new thread std::thread t(startFileEncoding, filename, pow(2,26), 4);//2^26 ~ 67 MB
     * @param filename
     * @param preferred_memory_consumtion in Byte imprecise heap allocation preffered to be used by this function by the caller please test the settings on your system it will need a min of ~7kBytes but more will allow faster and better encoding
     * @param numberThreads (if 0 then std thread hardware concurency value is used)
     */
    void startFileEncoding(const std::string filename, 
            const std::size_t preferred_memory_consumtion = pow(2,26),
            const std::size_t numberThreads = 4);

}//end namespace
#endif /* FILEENCODINGTASK_H */

