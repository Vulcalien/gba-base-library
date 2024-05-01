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

#define TIMER_COUNT (4)

#define TIMER0 (0)
#define TIMER1 (1)
#define TIMER2 (2)
#define TIMER3 (3)

struct Timer {
    u16 prescaler : 2; // 0=1, 1=64, 2=256, 3=1024 (number of cycles)
    u16 cascade   : 1; // 0=disable, 1=enable
    u16 _unused_0 : 3;
    u16 irq       : 1; // 0=disable, 1=enable (IRQ on timer overflow)
    u16 enable    : 1; // 0=disable, 1=enable
    u16 _unused_1 : 8;
};

extern void timer_config(u32 id, const struct Timer *config);
extern void timer_stop(u32 id);

extern u16  timer_get_remaining(u32 id);
extern void timer_set_remaining(u32 id, u16 remaining);