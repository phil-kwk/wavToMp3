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
 * File:   MusikExample.h
 * Author: Philipp Kaaden
 *
 * Created on 15. November 2020, 12:08
 */

#ifndef MUSIKEXAMPLE_H
#define MUSIKEXAMPLE_H

#include <cstdint>
#include <vector>

#include "../src/Formats.h"

namespace encoder {
    const WAVE_Format testRiff_WAVE_FORMAT();
    const std::vector<uint8_t> testRiff_Buffer();
    const std::vector<int16_t> testRiff_left_samples();
    const std::vector<int16_t> testRiff_right_samples();

    const WAVE_Format testMusic_WAVE_FORMAT();
    const std::vector<uint8_t> testMusic_Buffer();
    const std::vector<int16_t> testMusic_left_samples();
    const std::vector<int16_t> testMusic_right_samples();

    const WAVE_Format testfile_WAVE_FORMAT();
    const std::vector<uint8_t> testfile_Buffer();
    const std::vector<int16_t> testfile_right_samples();
    const std::vector<int16_t> testfile_left_samples();
    const ChannelContainer<int16_t> testfile_ChannelContainerSamples();
}

#endif /* MUSIKEXAMPLE_H */

