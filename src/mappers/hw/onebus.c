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

static void (*WSync)(void);

static writefunc defapuwrite[64];
static readfunc defapuread[64];

void (*OneBus_pwrap)(uint16 A, uint16 V);
void (*OneBus_cwrap)(uint16 A, uint16 V);

ONEBUS onebus;

static SFORMAT StateRegs[] =
{
	{ onebus.cpu41xx, 0x100, "REGC" },
	{ onebus.ppu20xx, 0x100, "REGS" },
	{ onebus.apu40xx, 0x40, "REGA" },
	{ &onebus.IRQReload, 1, "IRQR" },
	{ &onebus.IRQCount, 1, "IRQC" },
	{ &onebus.IRQa, 1, "IRQA" },
	{ &onebus.pcm_enable, 1, "PCME" },
	{ &onebus.pcm_irq, 1, "PCMI" },
	{ &onebus.pcm_addr, 2, "PCMA" },
	{ &onebus.pcm_size, 2, "PCMS" },
	{ &onebus.pcm_latch, 2, "PCML" },
	{ &onebus.pcm_clock, 2, "PCMC" },
	{ 0 }
};

static void OneBus_GENPWRAP(uint16 A, uint16 V) {
	setprg8(A, V);
}

static void OneBus_GENCWRAP(uint16 A, uint16 V) {
	setchr1(A, V);
}

static void OneBus_GENSYNC(void) {
	OneBus_FixPRG(0x0FFF, 0);
	OneBus_FixCHR(0x7FFF, 0);
	OneBus_FixMIR();
}

void OneBus_FixPRG(uint16 mmask, uint16 mblock) {
	uint8 bankmode = onebus.cpu41xx[0x0b] & 7;
	uint8 mask = (bankmode == 0x7) ? (0xff) : (0x3f >> bankmode);
	uint32 block = ((onebus.cpu41xx[0x00] & 0xf0) << 4) + (onebus.cpu41xx[0x0a] & (~mask));
	uint32 pswap = (onebus.cpu41xx[0x05] & 0x40) << 8;

#if 0
	uint8 bank0  = (onebus.cpu41xx[0x0b] & 0x40)?(~1):(onebus.cpu41xx[0x07]);
	uint8 bank1  = onebus.cpu41xx[0x08];
	uint8 bank2  = (onebus.cpu41xx[0x0b] & 0x40)?(onebus.cpu41xx[0x09]):(~1);
	uint8 bank3  = ~0;
#endif
	uint8 bank0 = onebus.cpu41xx[0x07];
	uint8 bank1 = onebus.cpu41xx[0x08];
	uint8 bank2 = (onebus.cpu41xx[0x0b] & 0x40) ? (onebus.cpu41xx[0x09]) : (~1);
	uint8 bank3 = ~0;

/*	FCEU_printf(" PRG: %04x [%02x]",0x8000^pswap,block | (bank0 & mask)); */
	OneBus_pwrap(0x8000 ^ pswap, mblock | ((block | (bank0 & mask)) & mmask));
/*	FCEU_printf(" %04x [%02x]",0xa000^pswap,block | (bank1 & mask)); */
	OneBus_pwrap(0xa000,         mblock | ((block | (bank1 & mask)) & mmask));
/*	FCEU_printf(" %04x [%02x]",0xc000^pswap,block | (bank2 & mask)); */
	OneBus_pwrap(0xc000 ^ pswap, mblock | ((block | (bank2 & mask)) & mmask));
/*	FCEU_printf(" %04x [%02x]\n",0xe000^pswap,block | (bank3 & mask)); */
	OneBus_pwrap(0xe000,         mblock | ((block | (bank3 & mask)) & mmask));
}

void OneBus_FixCHR(uint16 mmask, uint16 mblock) {
	static const uint8 midx[8] = { 0, 1, 2, 0, 3, 4, 5, 0 };
	uint8 mask = 0xff >> midx[onebus.ppu20xx[0x1a] & 7];
	uint32 block = ((onebus.cpu41xx[0] & 0x0f) << 11) + ((onebus.ppu20xx[0x18] & 0x70) << 4) + (onebus.ppu20xx[0x1a] & (~mask));
	uint32 cswap = (onebus.cpu41xx[0x05] & 0x80) << 5;

	uint8 bank0 = onebus.ppu20xx[0x16] & (~1);
	uint8 bank1 = onebus.ppu20xx[0x16] | 1;
	uint8 bank2 = onebus.ppu20xx[0x17] & (~1);
	uint8 bank3 = onebus.ppu20xx[0x17] | 1;
	uint8 bank4 = onebus.ppu20xx[0x12];
	uint8 bank5 = onebus.ppu20xx[0x13];
	uint8 bank6 = onebus.ppu20xx[0x14];
	uint8 bank7 = onebus.ppu20xx[0x15];

	OneBus_cwrap(0x0000 ^ cswap, mblock | ((block | (bank0 & mask)) & mmask));
	OneBus_cwrap(0x0400 ^ cswap, mblock | ((block | (bank1 & mask)) & mmask));
	OneBus_cwrap(0x0800 ^ cswap, mblock | ((block | (bank2 & mask)) & mmask));
	OneBus_cwrap(0x0C00 ^ cswap, mblock | ((block | (bank3 & mask)) & mmask));
	OneBus_cwrap(0x1000 ^ cswap, mblock | ((block | (bank4 & mask)) & mmask));
	OneBus_cwrap(0x1400 ^ cswap, mblock | ((block | (bank5 & mask)) & mmask));
	OneBus_cwrap(0x1800 ^ cswap, mblock | ((block | (bank6 & mask)) & mmask));
	OneBus_cwrap(0x1C00 ^ cswap, mblock | ((block | (bank7 & mask)) & mmask));
}

void OneBus_FixMIR(void) {
	setmirror((onebus.cpu41xx[0x06] ^ 1) & 1);
}

DECLFR(OneBus_ReadCPU41XX) {
	if ((A >= 0x4100) && (A <= 0x4FFF)) {
		switch (A & 0x0FFF) {
		case 0x140: case 0x141: case 0x142: case 0x143: case 0x144: case 0x145: case 0x146: case 0x147:
		case 0x148: case 0x149: case 0x14A: case 0x14B: case 0x14C: case 0x14D: case 0x14E: case 0x14F:
		case 0x150: case 0x151: case 0x152: case 0x153: case 0x154: case 0x155: case 0x156: case 0x157:
		case 0x158: case 0x159: case 0x15A: case 0x15B: case 0x15D: case 0x15E: case 0x15F:
			/* TODO: VT369 gpio */
			return 0xFF;
		case 0x15C:
			return (0x10);
		case 0x18A:
			return (0x04);
		case 0x1B7:
			return (0x04);
		case 0x1B9:
			return (0x80);
		default:
			if ((A <= 0x410D) || ((A >= 0x4160) && (A < 0x4800))) {
				return (onebus.cpu41xx[A & 0xFF]);
			}
			return (CartBR(A));
		}
	}
}

DECLFW(OneBus_WriteCPU41XX) {
	/*	FCEU_printf("CPU %04x:%04x\n",A,V); */
	if ((A >= 0x4100) && (A <= 0x41FF)) {
		switch (A & 0x1FF) {
		case 0x101:
			onebus.cpu41xx[0x01] = V & 0xfe;
			break;
		case 0x102:
			onebus.IRQReload = 1;
			break;
		case 0x103:
			X6502_IRQEnd(FCEU_IQEXT);
			onebus.IRQa = 0;
			break;
		case 0x104:
			onebus.IRQa = 1;
			break;
		case 0x140: case 0x141: case 0x142: case 0x143: case 0x144: case 0x145: case 0x146: case 0x147:
		case 0x148: case 0x149: case 0x14A: case 0x14B: case 0x14C: case 0x14D: case 0x14E: case 0x14F:
		case 0x150: case 0x151: case 0x152: case 0x153: case 0x154: case 0x155: case 0x156: case 0x157:
		case 0x158: case 0x159: case 0x15A: case 0x15B: case 0x15C: case 0x15D: case 0x15E: case 0x15F:
			/* VT369 gpio write */
			break;
		case 0x160:
		case 0x161:
			/* VT369 relative address */
			break;
		}
		onebus.cpu41xx[A & 0xFF] = V;
		WSync();
	}
}

DECLFW(OneBus_WritePPU20XX) {
	/*	FCEU_printf("PPU %04x:%04x\n",A,V); */
	if ((A >= 0x2010) && (A <= 0x20FF)) {
		onebus.ppu20xx[A & 0xFF] = V;
		WSync();
	}
}

DECLFW(OneBus_WriteMMC3) {
	/*	FCEU_printf("MMC %04x:%04x\n",A,V); */
	switch (A & 0xe001) {
	case 0x8000:
		OneBus_WriteCPU41XX(0x4105, V & ~0x20);
		break;
	case 0x8001: {
		uint8 reg = onebus.cpu41xx[0x05] & 7;
		switch (reg) {
		case 0:
		case 1:
			OneBus_WritePPU20XX(0x2016 + reg, V);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			OneBus_WritePPU20XX(0x2010 + reg, V);
			break;
		case 6:
		case 7:
			OneBus_WriteCPU41XX(0x4101 + reg, V);
			break;
		}
		break;
	}
	case 0xa000:
		OneBus_WriteCPU41XX(0x4106, V);
		break;
	case 0xc000:
		OneBus_WriteCPU41XX(0x4101, V);
		break;
	case 0xc001:
		OneBus_WriteCPU41XX(0x4102, V);
		break;
	case 0xe000:
		OneBus_WriteCPU41XX(0x4103, V);
		break;
	case 0xe001:
		OneBus_WriteCPU41XX(0x4104, V);
		break;
	}
}

static void OneBus_IRQHook(void) {
	uint32 count = onebus.IRQCount;
	if (!count || onebus.IRQReload) {
		onebus.IRQCount = onebus.cpu41xx[0x01];
		onebus.IRQReload = 0;
	} else
		onebus.IRQCount--;
	if (count && !onebus.IRQCount) {
		if (onebus.IRQa)
			X6502_IRQBegin(FCEU_IQEXT);
	}
}

DECLFW(OneBus_WriteAPU40XX) {
	/* if(((A & 0x3f)!=0x16) && ((onebus.apu40xx[0x30] & 0x10) || ((A & 0x3f)>0x17)))FCEU_printf("APU %04x:%04x\n",A,V);  */
	if ((A >= 0x4000) && (A <= 0x403F)) {
		onebus.apu40xx[A & 0x3f] = V;
		switch (A & 0x3f) {
		case 0x12:
			if (onebus.apu40xx[0x30] & 0x10) {
				onebus.pcm_addr = V << 6;
			}
			break;
		case 0x13:
			if (onebus.apu40xx[0x30] & 0x10) {
				onebus.pcm_size = (V << 4) + 1;
			}
			break;
		case 0x15:
			if (onebus.apu40xx[0x30] & 0x10) {
				onebus.pcm_enable = V & 0x10;
				if (onebus.pcm_irq) {
					X6502_IRQEnd(FCEU_IQEXT);
					onebus.pcm_irq = 0;
				}
				if (onebus.pcm_enable) onebus.pcm_latch = onebus.pcm_clock;
				V &= 0xef;
			}
			break;
		}
		defapuwrite[A & 0x3f](A, V);
	}
}

DECLFR(OneBus_ReadAPU40XX) {
	uint8 result = defapuread[A & 0x3f](A);
/*	FCEU_printf("read %04x, %02x\n",A,result); */
	switch (A & 0x3f) {
	case 0x15:
		if (onebus.apu40xx[0x30] & 0x10) {
			result = (result & 0x7f) | onebus.pcm_irq;
		}
		break;
	}
	return result;
}

static void OneBus_CpuHook(int a) {
	if (onebus.pcm_enable) {
		onebus.pcm_latch -= a;
		if (onebus.pcm_latch <= 0) {
			onebus.pcm_latch += onebus.pcm_clock;
			onebus.pcm_size--;
			if (onebus.pcm_size < 0) {
				onebus.pcm_irq = 0x80;
				onebus.pcm_enable = 0;
				X6502_IRQBegin(FCEU_IQEXT);
			} else {
				uint16 addr = onebus.pcm_addr | ((onebus.apu40xx[0x30] ^ 3) << 14);
				uint8 raw_pcm = ARead[addr](addr) >> 1;
				defapuwrite[0x11](0x4011, raw_pcm);
				onebus.pcm_addr++;
				onebus.pcm_addr &= 0x7FFF;
			}
		}
	}
}

void OneBus_Power(void) {
	uint32 i;

	SetupCartCHRMapping(0, PRGptr[0], PRGsize[0], 0);

	for (i = 0; i < 64; i++) {
		defapuread[i] = GetReadHandler(0x4000 | i);
		defapuwrite[i] = GetWriteHandler(0x4000 | i);
	}
	SetReadHandler(0x4000, 0x403f, OneBus_ReadAPU40XX);
	SetWriteHandler(0x4000, 0x403f, OneBus_WriteAPU40XX);

	SetReadHandler(0x6000, 0xFFFF, CartBR);
	SetWriteHandler(0x6000, 0x7FFF, CartBW);
	SetWriteHandler(0x2010, 0x20FF, OneBus_WritePPU20XX);
	
	SetReadHandler(0x4100, 0x4FFF, OneBus_ReadCPU41XX);
	SetWriteHandler(0x4100, 0x41FF, OneBus_WriteCPU41XX);
	
	SetWriteHandler(0x8000, 0xffff, OneBus_WriteMMC3);

	FCEU_CheatAddRAM(8, 0x6000, WRAM);
	setprg8r(0x10, 0x6000, 0);
	
	memset(onebus.cpu41xx, 0x00, sizeof(onebus.cpu41xx));
	memset(onebus.ppu20xx, 0x00, sizeof(onebus.ppu20xx));
	memset(onebus.apu40xx, 0x00, sizeof(onebus.apu40xx));

	onebus.pcm_enable = onebus.pcm_irq = 0;
	onebus.pcm_addr = onebus.pcm_size = onebus.pcm_latch = 0;
	onebus.pcm_clock = 0xE1;

	OneBus_Reset();
}

void OneBus_Reset(void) {
	onebus.IRQReload = onebus.IRQCount = onebus.IRQa = 0;

	onebus.ppu20xx[0x10] = 0x00;
	onebus.ppu20xx[0x12] = 0x04;
	onebus.ppu20xx[0x13] = 0x05;
	onebus.ppu20xx[0x14] = 0x06;
	onebus.ppu20xx[0x15] = 0x07;
	onebus.ppu20xx[0x16] = 0x00;
	onebus.ppu20xx[0x17] = 0x02;
	onebus.ppu20xx[0x18] = 0x00;
	onebus.ppu20xx[0x1A] = 0x00;
	onebus.cpu41xx[0x00] = 0x00;
	onebus.cpu41xx[0x05] = 0x00;
	onebus.cpu41xx[0x07] = 0x00;
	onebus.cpu41xx[0x08] = 0x01;
	onebus.cpu41xx[0x09] = 0xFE;
	onebus.cpu41xx[0x0A] = 0x00;
	onebus.cpu41xx[0x0B] = 0x00;
	onebus.cpu41xx[0x0F] = 0xFF;
	onebus.cpu41xx[0x60] = 0x00;
	onebus.cpu41xx[0x61] = 0x00;

	WSync();
}

static void StateRestore(int version) {
	WSync();
}

static void OneBus_Close(void) {
}

void OneBus_Init(CartInfo *info, void (*proc)(void), int wram, int battery) {
	OneBus_pwrap = OneBus_GENPWRAP;
	OneBus_cwrap = OneBus_GENCWRAP;

	WSync = proc;

	info->Power = OneBus_Power;
	info->Reset = OneBus_Reset;
	info->Close = OneBus_Close;

	if (wram) {
		WRAMSIZE = wram * 1024;
		WRAM = (uint8*)FCEU_gmalloc(WRAMSIZE);
		SetupCartPRGMapping(0x10, WRAM, WRAMSIZE, 1);
		AddExState(WRAM, WRAMSIZE, 0, "WRAM");
		if (battery) {
			info->SaveGame[0] = WRAM;
			info->SaveGameLen[0] = WRAMSIZE;
		}
	}

	GameHBIRQHook = OneBus_IRQHook;
	MapIRQHook = OneBus_CpuHook;
	GameStateRestore = StateRestore;
	AddExState(StateRegs, ~0, 0, NULL);
}
