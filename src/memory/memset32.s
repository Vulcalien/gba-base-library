@ Copyright 2024 Vulcalien
@
@ This program is free software: you can redistribute it and/or modify
@ it under the terms of the GNU General Public License as published by
@ the Free Software Foundation, either version 3 of the License, or
@ (at your option) any later version.
@
@ This program is distributed in the hope that it will be useful,
@ but WITHOUT ANY WARRANTY; without even the implied warranty of
@ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
@ GNU General Public License for more details.
@
@ You should have received a copy of the GNU General Public License
@ along with this program. If not, see <https://www.gnu.org/licenses/>.

.include "macros.inc"

@ --- memset32 --- @
.global memset32
.text
THUMB_FUNC

@ Fill a memory area with a given byte value: 32-bit version.
@
@ Only the lowest 8 bits of 'byte' will be considered. The highest
@ 24 bits will be ignored.
@
@ 'dest' must be aligned to 4.
@ 'n' should be a multiple of 4, but its lowest 2 bits are ignored.

@ input:
@   r0 = dest : pointer
@   r1 = byte : signed 32-bit
@   r2 = n    : unsigned 32-bit
@ output:
@   r0 = dest : pointer
memset32:
    push    {r0, r4-r6}

    @ duplicate 'byte' to fill 32 bits
    lsl     r3, r1, #8                  @ r3 = 00 00 xx 00
    orr     r1, r3                      @ r1 = 00 00 xx xx
    lsl     r3, r1, #16                 @ r3 = xx xx 00 00
    orr     r1, r3                      @ r1 = xx xx xx xx

    @ calculate number of words and 4-word blocks
    lsr     r2, #2                      @ (r2) n /= 4
    lsr     r3, r2, #2                  @ (r3) blocks = n / 4

    @ if blocks == 0, skip the multi-store loop
    beq     2f @ exit multi-store loop

    @ copy content word into r4-r6
    mov     r4, r1                      @ r4 = content
    mov     r5, r1                      @ r5 = content
    mov     r6, r1                      @ r6 = content

1: @ multi-store loop
    stmia   r0!, {r1, r4-r6}            @ (r0) dest += 16
    sub     r3, #1                      @ (r3) blocks -= 1
    bne     1b @ multi-store loop       @ if blocks != 0, repeat loop
2: @ exit multi-store loop

    @ calculate n % 4
    lsl     r2, #30
    lsr     r2, #30

    @ if n == 0, skip the single-store loop
    beq     4f @ exit single-store loop

3: @ single-store loop
    stmia   r0!, {r1}                   @ (r0) dest += 4
    sub     r2, #1                      @ (r2) n -= 1
    bne     3b @ single-store loop      @ if n != 0, repeat loop
4: @ exit single-store loop

    @ return original value of dest
    pop     {r0, r4-r6}
    bx      lr

.size memset32, .-memset32

.end
