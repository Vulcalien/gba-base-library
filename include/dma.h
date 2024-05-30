/* Copyright 2024 Vulcalien
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include "base.h"

#define DMA_COUNT 4

#define DMA0 0
#define DMA1 1
#define DMA2 2
#define DMA3 3

#define _DMA_GET_CONTROL(id) ((vu16 *) (0x040000ba + id * 12))

struct DMA {
    u16 dest_control : 2; // 0=increment, 1=decrement, 2=fixed, 3=reload
    u16 src_control  : 2; // 0=increment, 1=decrement, 2=fixed
    u16 repeat       : 1; // 0=disable, 1=enable
    u16 size         : 1; // 0=16-bit, 1=32-bit
    u16 start_timing : 2; // 0=immediately, 1=VBlank, 2=HBlank, 3=special
    u16 irq          : 1; // 0=disable, 1=enable
};

INLINE void dma_config(u32 id, struct DMA *config) {
    if(id >= DMA_COUNT)
        return;

    vu16 *control = _DMA_GET_CONTROL(id);
    if(config) {
        *control = config->dest_control << 5  |
                   config->src_control  << 7  |
                   config->repeat       << 9  |
                   config->size         << 10 |
                   config->start_timing << 12 |
                   config->irq          << 14;
    } else {
        *control = 0;
    }
}

INLINE void dma_transfer(u32 id,
                         volatile void *dest, volatile const void *src,
                         u32 n) {
    if(id >= DMA_COUNT)
        return;

    vu32 *src_addr   = (vu32 *) (0x040000b0 + id * 12);
    vu32 *dest_addr  = (vu32 *) (0x040000b4 + id * 12);
    vu16 *word_count = (vu16 *) (0x040000b8 + id * 12);

    *src_addr   = (u32) src;
    *dest_addr  = (u32) dest;
    *word_count = n;

    vu16 *control = _DMA_GET_CONTROL(id);
    *control |= (1 << 15);
}

INLINE void dma_stop(u32 id) {
    if(id >= DMA_COUNT)
        return;

    vu16 *control = _DMA_GET_CONTROL(id);
    *control &= ~(1 << 15);
}

#undef _DMA_GET_CONTROL
