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
#include <mutex>

#include "ReadFile.h"
#include "WriteFile.h"
#include "Formats.h"

#ifndef FILEENCODINGTASK_H
#define FILEENCODINGTASK_H

namespace encoder {
    
    /**
     * @hint start in new thread std::thread t(startFileEncoding, filename, 10000000, 4);
     * @param filename
     * @param max_memory_consumtion
     * @param numberThreads
     */
    void startFileEncoding(const std::string filename, 
            const std::size_t max_memory_consumtion = 10000000,
            const std::size_t numberThreads = 4);

}//end namespace
#endif /* FILEENCODINGTASK_H */

