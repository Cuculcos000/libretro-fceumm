/* FCE Ultra - NES/Famicom Emulator
 *
 * Copyright notice for this file:
 *  Copyright (C) 2007-2010 CaH4e3
 *  Copyright (C) 2025 negativeExponent
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
 *
 * VR02/VT03 Console and OneBus System
 *
 * Street Dance (Dance pad) (Unl)
 * 101-in-1 Arcade Action II
 * DreamGEAR 75-in-1, etc.
 *
 */

#include "mapinc.h"
#include "onebus.h"

static const uint8 cpuMangle[16][4] = {
	{ 0, 1, 2, 3 }, /* Submapper 0: Normal                                  */
	{ 0, 1, 2, 3 }, /* Submapper 1: Waixing VT03                            */
	{ 1, 0, 2, 3 }, /* Submapper 2: Trump Grand                             */
	{ 0, 1, 2, 3 }, /* Submapper 3: Zechess                                 */
	{ 0, 1, 2, 3 }, /* Submapper 4: Qishenglong                             */
	{ 0, 1, 2, 3 }, /* Submapper 5: Waixing VT02                            */
	{ 0, 1, 2, 3 }, /* Submapper 6: unused so far                           */
	{ 0, 1, 2, 3 }, /* Submapper 7: unused so far                           */
	{ 0, 1, 2, 3 }, /* Submapper 8: unused so far                           */
	{ 0, 1, 2, 3 }, /* Submapper 9: unused so far                           */
	{ 0, 1, 2, 3 }, /* Submapper A: unused so far                           */
	{ 0, 1, 2, 3 }, /* Submapper B: unused so far                           */
	{ 0, 1, 2, 3 }, /* Submapper C: unused so far                           */
	{ 0, 1, 2, 3 }, /* Submapper D: Cube Tech (CPU opcode encryption only)  */
	{ 0, 1, 2, 3 }, /* Submapper E: Karaoto (CPU opcode encryption only)    */
	{ 0, 1, 2, 3 } /* Submapper F: Jungletac (CPU opcode encryption only)  */
};

static const uint8 ppuMangle[16][6] = {
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper 0: Normal                                  */
	{ 1, 0, 5, 4, 3, 2 }, /* Submapper 1: Waixing VT03                            */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper 2: Trump Grand                             */
	{ 5, 4, 3, 2, 0, 1 }, /* Submapper 3: Zechess                                 */
	{ 2, 5, 0, 4, 3, 1 }, /* Submapper 4: Qishenglong                             */
	{ 1, 0, 5, 4, 3, 2 }, /* Submapper 5: Waixing VT02                            */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper 6: unused so far                           */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper 7: unused so far                           */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper 8: unused so far                           */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper 9: unused so far                           */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper A: unused so far                           */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper B: unused so far                           */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper C: unused so far                           */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper D: Cube Tech (CPU opcode encryption only)  */
	{ 0, 1, 2, 3, 4, 5 }, /* Submapper E: Karaoto (CPU opcode encryption only)    */
	{ 0, 1, 2, 3, 4, 5 } /* Submapper F: Jungletac (CPU opcode encryption only)  */
};

static const uint8 mmc3Mangle[16][8] = {
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper 0: Normal                                 */
	{ 5, 4, 3, 2, 1, 0, 6, 7 }, /* Submapper 1: Waixing VT03                           */
	{ 0, 1, 2, 3, 4, 5, 7, 6 }, /* Submapper 2: Trump Grand                            */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper 3: Zechess                                */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper 4: Qishenglong                            */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper 5: Waixing VT02                           */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper 6: unused so far                          */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper 7: unused so far                          */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper 8: unused so far                          */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper 9: unused so far                          */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper A: unused so far                          */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper B: unused so far                          */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper C: unused so far                          */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper D: Cube Tech (CPU opcode encryption only) */
	{ 0, 1, 2, 3, 4, 5, 6, 7 }, /* Submapper E: Karaoto (CPU opcode encryption only)   */
	{ 0, 1, 2, 3, 4, 5, 6, 7 } /* Submapper F: Jungletac (CPU opcode encryption only) */
};

static void M256Sync(void) {
	OneBus_FixPRG(0x0FFF, 0);
	OneBus_FixCHR(0x7FFF, 0);
	OneBus_FixMIR();
}

static DECLFW(M256WritePPU201X) {
	A = 0x2012 + ppuMangle[iNESCart.submapper][A - 0x2012];
	OneBus_WritePPU20XX(A, V);
}

static DECLFW(M256WriteCPU410X) {
	A = 0x4107 + cpuMangle[iNESCart.submapper][A - 0x4107];
	OneBus_WriteCPU41XX(A, V);
}

static DECLFW(M256WriteMMC3) {
	V = (V & 0xF8) | mmc3Mangle[iNESCart.submapper][V & 0x07];
	OneBus_WriteMMC3(A, V);
}

static void M256Power(void) {
	OneBus_Power();
	SetWriteHandler(0x2012, 0x2017, M256WritePPU201X);
	SetWriteHandler(0x4107, 0x410A, M256WriteCPU410X);
	SetWriteHandler(0x8000, 0x9FFF, M256WriteMMC3);
}

void Mapper256_Init(CartInfo *info) {
	int ws = (info->PRGRamSize + info->PRGRamSaveSize) / 1024;

	if (!ws) {
		ws = 8;
	}

	if (!info->iNES2) {
		iNESCart.submapper = (((*(uint32 *)&(info->MD5)) == 0x305fcdc3) || ((*(uint32 *)&(info->MD5)) == 0x6abfce8e)) ? 2 : 0; /* PowerJoy Supermax Carts */
	}

	OneBus_Init(info, M256Sync, ws, info->battery);
	info->Power = M256Power;
}
