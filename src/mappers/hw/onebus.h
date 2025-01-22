/* FCEUmm - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2023-2025 negativeExponent
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _ONEBUS_H
#define _ONEBUS_H

typedef struct __ONEBUS {
    /* General Purpose Registers */
    uint8 cpu41xx[0x0100], ppu20xx[0x100], apu40xx[0x40];

    /* IRQ Registers */
    uint8 IRQCount, IRQa, IRQReload;

    /* APU Registers */
    uint8 pcm_enable, pcm_irq;
    int16 pcm_addr, pcm_size, pcm_latch, pcm_clock;
} ONEBUS;

extern ONEBUS onebus;

DECLFW(OneBus_WriteCPU41XX); /* CPU Write $4100 - $41FF */
DECLFW(OneBus_WritePPU20XX); /* PPU Write $2010 - $20FF */
DECLFW(OneBus_WriteMMC3);
DECLFW(OneBus_WriteAPU40XX); /* APU Write $4000 - $403F */
DECLFR(OneBus_ReadAPU40XX);  /* APU Read  $4000 - $403F */

void OneBus_Power(void);
void OneBus_Reset(void);
void OneBus_Init(CartInfo *info, void (*proc)(void), int wram, int battery);

void OneBus_FixPRG(uint16 mmask, uint16 mblock);
void OneBus_FixCHR(uint16 mmask, uint16 mblock);
void OneBus_FixMIR(void);

extern void (*OneBus_pwrap)(uint16 A, uint16 V);
extern void (*OneBus_cwrap)(uint16 A, uint16 V);

#endif /* _ONEBUS_H */
