/*******************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
 
  (c) Copyright 1996 - 2003 Gary Henderson (gary.henderson@ntlworld.com) and
                            Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2003 Matthew Kendora and
                            Brad Jorsch (anomie@users.sourceforge.net)
 

                      
  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003 zsKnight (zsknight@zsnes.com),
                            _Demo_ (_demo_@zsnes.com), and
                            Nach (n-a-c-h@users.sourceforge.net)
                                          
  C4 C++ code
  (c) Copyright 2003 Brad Jorsch

  DSP-1 emulator code
  (c) Copyright 1998 - 2003 Ivar (ivar@snes9x.com), _Demo_, Gary Henderson,
                            John Weidman (jweidman@slip.net),
                            neviksti (neviksti@hotmail.com), and
                            Kris Bleakley (stinkfish@bigpond.com)
 
  DSP-2 emulator code
  (c) Copyright 2003 Kris Bleakley, John Weidman, neviksti, Matthew Kendora, and
                     Lord Nightmare (lord_nightmare@users.sourceforge.net

  OBC1 emulator code
  (c) Copyright 2001 - 2003 zsKnight, pagefault (pagefault@zsnes.com)
  Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002 Matthew Kendora with research by
                     zsKnight, John Weidman, and Dark Force

  S-RTC C emulator code
  (c) Copyright 2001 John Weidman
  
  Super FX x86 assembler emulator code 
  (c) Copyright 1998 - 2003 zsKnight, _Demo_, and pagefault 

  Super FX C emulator code 
  (c) Copyright 1997 - 1999 Ivar and Gary Henderson.



 
  Specific ports contains the works of other authors. See headers in
  individual files.
 
  Snes9x homepage: http://www.snes9x.com
 
  Permission to use, copy, modify and distribute Snes9x in both binary and
  source form, for non-commercial purposes, is hereby granted without fee,
  providing that this license information and copyright notice appear with
  all copies and any derived work.
 
  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software.
 
  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes
  charging money for Snes9x or software derived from Snes9x.
 
  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.
 
  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
*******************************************************************************/

#include "snes9x.h"
#include "memmap.h"
#include "ppu.h"
#include "dsp1.h"
#include "cpuexec.h"
#include "debug.h"
#include "apu.h"
#include "dma.h"
#include "sa1.h"
#include "cheats.h"
#include "srtc.h"
#include "sdd1.h"
#include "spc7110.h"
#include "obc1.h"


#ifndef ZSNES_FX
#include "fxemu.h"

extern struct FxInit_s SuperFX;

void S9xResetSuperFX ()
{
    SuperFX.vFlags = 0; //FX_FLAG_ROM_BUFFER;// | FX_FLAG_ADDRESS_CHECKING;
    FxReset (&SuperFX);
}
#endif

void S9xResetCPU ()
{
    Registers.PB = 0;
    Registers.PC = S9xGetWord (0xFFFC);
    Registers.D.W = 0;
    Registers.DB = 0;
    Registers.SH = 1;
    Registers.SL = 0xFF;
    Registers.XH = 0;
    Registers.YH = 0;
    Registers.P.W = 0;

    ICPU.ShiftedPB = 0;
    ICPU.ShiftedDB = 0;
    SetFlags (MemoryFlag | IndexFlag | IRQ | Emulation);
    ClearFlags (Decimal);

    CPU.Flags = CPU.Flags & (DEBUG_MODE_FLAG | TRACE_FLAG);
    CPU.BranchSkip = FALSE;
    CPU.NMIActive = FALSE;
    CPU.IRQActive = FALSE;
    CPU.WaitingForInterrupt = FALSE;
    CPU.InDMA = FALSE;
    CPU.WhichEvent = HBLANK_START_EVENT;
    CPU.PC = NULL;
    SCHERZO_PRINT("S9xResetCPU: CPU.PC = NULL\n");
    CPU.PCBase = NULL;
    CPU.PCAtOpcodeStart = NULL;
    CPU.WaitAddress = NULL;
    CPU.WaitCounter = 0;
    CPU.Cycles = 0;
    CPU.NextEvent = Settings.HBlankStart;
    CPU.V_Counter = 0;
    CPU.MemSpeed = SLOW_ONE_CYCLE;
    CPU.MemSpeedx2 = SLOW_ONE_CYCLE * 2;
    CPU.FastROMSpeed = SLOW_ONE_CYCLE;
    CPU.AutoSaveTimer = 0;
    CPU.SRAMModified = FALSE;
    // CPU.NMITriggerPoint = 4; // Set when ROM image loaded
    CPU.BRKTriggered = FALSE;
    //CPU.TriedInterleavedMode2 = FALSE; // Reset when ROM image loaded
    CPU.NMICycleCount = 0;
    CPU.IRQCycleCount = 0;
    S9xSetPCBase (Registers.PC);

    ICPU.S9xOpcodes = S9xOpcodesM1X1;
    ICPU.CPUExecuting = TRUE;

    S9xUnpackStatus();
}

#ifdef ZSNES_FX
START_EXTERN_C
void S9xResetSuperFX ();
bool8 WinterGold = 0;
extern uint8 *C4Ram;
END_EXTERN_C
#endif

void S9xReset (void)
{
    if (Settings.SuperFX)
        S9xResetSuperFX ();

#ifdef ZSNES_FX
    WinterGold = Settings.WinterGold;
#endif
    ZeroMemory (Memory.FillRAM, 0x8000);
    memset (Memory.VRAM, 0x00, 0x10000);
    memset (Memory.RAM, 0x55, 0x20000);

	if(Settings.SPC7110)
		S9xSpc7110Reset();
    S9xResetCPU ();
    S9xResetPPU ();
    S9xResetSRTC ();
    if (Settings.SDD1)
        S9xResetSDD1 ();

    S9xResetDMA ();
    S9xResetAPU ();
    S9xResetDSP1 ();
    S9xSA1Init ();
    if (Settings.C4)
        S9xInitC4 ();
    S9xInitCheatData ();
	if(Settings.OBC1)
		ResetOBC1();

    Settings.Paused = FALSE;
}
void S9xSoftReset (void)
{
    if (Settings.SuperFX)
        S9xResetSuperFX ();

#ifdef ZSNES_FX
    WinterGold = Settings.WinterGold;
#endif
    ZeroMemory (Memory.FillRAM, 0x8000);
    memset (Memory.VRAM, 0x00, 0x10000);
 //   memset (Memory.RAM, 0x55, 0x20000);

	if(Settings.SPC7110)
		S9xSpc7110Reset();
    S9xResetCPU ();
    S9xSoftResetPPU ();
    S9xResetSRTC ();
    if (Settings.SDD1)
        S9xResetSDD1 ();

    S9xResetDMA ();
    S9xResetAPU ();
    S9xResetDSP1 ();
	if(Settings.OBC1)
		ResetOBC1();
    S9xSA1Init ();
    if (Settings.C4)
        S9xInitC4 ();
    S9xInitCheatData ();

    Settings.Paused = FALSE;
}
