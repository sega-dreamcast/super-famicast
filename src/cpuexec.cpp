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
#include "cpuops.h"
#include "ppu.h"
#include "cpuexec.h"
#include "debug.h"
#include "snapshot.h"
#include "gfx.h"
#include "missing.h"
#include "apu.h"
#include "dma.h"
#include "fxemu.h"
#include "sa1.h"
#include "spc7110.h"

//uint32 g_code_counts[0x100];

inline void ExecOpCode()
{
	
}

void S9xMainLoop (void)
{
	//uint32 times_through = 0;
    for (;;)
    {
    	//++times_through;
		APU_EXECUTE();
		if (CPU.Flags)
		{
	    	if (CPU.Flags & NMI_FLAG)
	    	{
				if (--CPU.NMICycleCount == 0)
				{
		    		CPU.Flags &= ~NMI_FLAG;
		    		if (CPU.WaitingForInterrupt)
		    		{
						CPU.WaitingForInterrupt = FALSE;
						CPU.PC++;
		    		}
		    		S9xOpcode_NMI();
				}
	    	}
			
	    	CHECK_SOUND ();

	    	if (CPU.Flags & IRQ_PENDING_FLAG)
	    	{
				if (CPU.IRQCycleCount == 0)
				{
					if (CPU.WaitingForInterrupt)
					{
						CPU.WaitingForInterrupt = FALSE;
						CPU.PC++;
		    		}
		    		if (CPU.IRQActive)
		    		{
						if (!CheckFlag (IRQ))
							S9xOpcode_IRQ();
		    		}
		    		else
						CPU.Flags &= ~IRQ_PENDING_FLAG;
				}
				else
				{
					if(--CPU.IRQCycleCount==0 && CheckFlag (IRQ))
						CPU.IRQCycleCount=1;
				}
	    	}

			if (CPU.Flags & SCAN_KEYS_FLAG)
				break;
		}
#ifdef CPU_SHUTDOWN
		CPU.PCAtOpcodeStart = CPU.PC;
#endif
		CPU.Cycles += CPU.MemSpeed;
		
		//g_code_counts[*CPU.PC] += 1;

		(*ICPU.S9xOpcodes[*CPU.PC++].S9xOpcode) ();
	
		if (SA1.Executing)
			S9xSA1MainLoop();
		DO_HBLANK_CHECK();
    }
    //printf("times_through = %u\n", times_through);
    Registers.PC = CPU.PC - CPU.PCBase;
    S9xPackStatus();
    APURegisters.PC = IAPU.PC - IAPU.RAM;
    S9xAPUPackStatus();
    if (CPU.Flags & SCAN_KEYS_FLAG)
    {
	    S9xSyncSpeed();
		CPU.Flags &= ~SCAN_KEYS_FLAG;
    }
    if (CPU.BRKTriggered && Settings.SuperFX && !CPU.TriedInterleavedMode2)
    {
		CPU.TriedInterleavedMode2 = TRUE;
		CPU.BRKTriggered = FALSE;
		S9xDeinterleaveMode2();
    }
}



void S9xSetIRQ (uint32 source)
{
    CPU.IRQActive |= source;
    CPU.Flags |= IRQ_PENDING_FLAG;
    CPU.IRQCycleCount = 3;
    if (CPU.WaitingForInterrupt)
    {
	// Force IRQ to trigger immediately after WAI - 
	// Final Fantasy Mystic Quest crashes without this.
	CPU.IRQCycleCount = 0;
	CPU.WaitingForInterrupt = FALSE;
	CPU.PC++;
    }
}

void S9xClearIRQ (uint32 source)
{
    CLEAR_IRQ_SOURCE (source);
}

void S9xDoHBlankProcessing ()
{
#ifdef CPU_SHUTDOWN
    CPU.WaitCounter++;
#endif
    switch (CPU.WhichEvent)
    {
    case HBLANK_START_EVENT:
	if (IPPU.HDMA && CPU.V_Counter <= PPU.ScreenHeight)
	    IPPU.HDMA = S9xDoHDMA (IPPU.HDMA);

	break;

    case HBLANK_END_EVENT:
	S9xSuperFXExec ();

	S9xGenerateSound ();

	CPU.Cycles -= Settings.H_Max;
	if (IAPU.APUExecuting)
	{
	    APU.Cycles -= Settings.H_Max;
#ifdef MK_APU
		S9xCatchupCount();
#endif
	}
	else
	    APU.Cycles = 0;

	CPU.NextEvent = -1;
	ICPU.Scanline++;

	if (++CPU.V_Counter >= (Settings.PAL ? SNES_MAX_PAL_VCOUNTER : SNES_MAX_NTSC_VCOUNTER))
	{
		CPU.V_Counter = 0;
		Memory.FillRAM[0x213F]^=0x80;
		PPU.RangeTimeOver = 0;
	    CPU.NMIActive = FALSE;
	    ICPU.Frame++;
	    PPU.HVBeamCounterLatched = 0;
	    CPU.Flags |= SCAN_KEYS_FLAG;
	    S9xStartHDMA ();
	}

	if (PPU.VTimerEnabled && !PPU.HTimerEnabled &&
	    CPU.V_Counter == PPU.IRQVBeamPos)
	{
	    S9xSetIRQ (PPU_V_BEAM_IRQ_SOURCE);
	}

	if (CPU.V_Counter == PPU.ScreenHeight + FIRST_VISIBLE_LINE)
	{
	    // Start of V-blank
	    S9xEndScreenRefresh ();
	    IPPU.HDMA = 0;
	    // Bits 7 and 6 of $4212 are computed when read in S9xGetPPU.
	    missing.dma_this_frame = 0;
	    IPPU.MaxBrightness = PPU.Brightness;
	    PPU.ForcedBlanking = (Memory.FillRAM [0x2100] >> 7) & 1;

		if(!PPU.ForcedBlanking){
			PPU.OAMAddr = PPU.SavedOAMAddr;
			{
				uint8 tmp = 0;
				if(PPU.OAMPriorityRotation)
					tmp = (PPU.OAMAddr&0xFE)>>1;
				if((PPU.OAMFlip&1) || PPU.FirstSprite!=tmp){
					PPU.FirstSprite=tmp;
					IPPU.OBJChanged=TRUE;
				}
			}
			PPU.OAMFlip = 0;
		}

	    Memory.FillRAM[0x4210] = 0x80 |MAX_5A22_VERSION;
	    if (Memory.FillRAM[0x4200] & 0x80)
	    {
			CPU.NMIActive = TRUE;
			CPU.Flags |= NMI_FLAG;
			CPU.NMICycleCount = CPU.NMITriggerPoint;
	    }

#ifdef OLD_SNAPSHOT_CODE
	    if (CPU.Flags & SAVE_SNAPSHOT_FLAG)
		{
			CPU.Flags &= ~SAVE_SNAPSHOT_FLAG;
			Registers.PC = CPU.PC - CPU.PCBase;
			S9xPackStatus ();
			S9xAPUPackStatus ();
			Snapshot (NULL);
		}
#endif
	}

	if (CPU.V_Counter == PPU.ScreenHeight + 3)
	    S9xUpdateJoypads ();

	if (CPU.V_Counter == FIRST_VISIBLE_LINE)
	{
	    Memory.FillRAM[0x4210] = MAX_5A22_VERSION;
	    CPU.Flags &= ~NMI_FLAG;
	    S9xStartScreenRefresh ();
	}
	if (CPU.V_Counter >= FIRST_VISIBLE_LINE &&
	    CPU.V_Counter < PPU.ScreenHeight + FIRST_VISIBLE_LINE)
	{
	    RenderLine (CPU.V_Counter - FIRST_VISIBLE_LINE);
	}
	// Use TimerErrorCounter to skip update of SPC700 timers once
	// every 128 updates. Needed because this section of code is called
	// once every emulated 63.5 microseconds, which coresponds to
	// 15.750KHz, but the SPC700 timers need to be updated at multiples
	// of 8KHz, hence the error correction.
//	IAPU.TimerErrorCounter++;
//	if (IAPU.TimerErrorCounter >= )
//	    IAPU.TimerErrorCounter = 0;
//	else
	{
	    if (APU.TimerEnabled [2])
	    {
		APU.Timer [2] += 4;
		while (APU.Timer [2] >= APU.TimerTarget [2])
		{
		    IAPU.RAM [0xff] = (IAPU.RAM [0xff] + 1) & 0xf;
		    APU.Timer [2] -= APU.TimerTarget [2];
#ifdef SPC700_SHUTDOWN		
		    IAPU.WaitCounter++;
		    IAPU.APUExecuting = TRUE;
#endif		
		}
	    }
	    if (CPU.V_Counter & 1)
	    {
		if (APU.TimerEnabled [0])
		{
		    APU.Timer [0]++;
		    if (APU.Timer [0] >= APU.TimerTarget [0])
		    {
			IAPU.RAM [0xfd] = (IAPU.RAM [0xfd] + 1) & 0xf;
			APU.Timer [0] = 0;
#ifdef SPC700_SHUTDOWN		
			IAPU.WaitCounter++;
			IAPU.APUExecuting = TRUE;
#endif		    
		    }
		}
		if (APU.TimerEnabled [1])
		{
		    APU.Timer [1]++;
		    if (APU.Timer [1] >= APU.TimerTarget [1])
		    {
			IAPU.RAM [0xfe] = (IAPU.RAM [0xfe] + 1) & 0xf;
			APU.Timer [1] = 0;
#ifdef SPC700_SHUTDOWN		
			IAPU.WaitCounter++;
			IAPU.APUExecuting = TRUE;
#endif		    
		    }
		}
	    }
	}
	break;
    case HTIMER_BEFORE_EVENT:
    case HTIMER_AFTER_EVENT:
	if (PPU.HTimerEnabled &&
	    (!PPU.VTimerEnabled || CPU.V_Counter == PPU.IRQVBeamPos))
	{
	    S9xSetIRQ (PPU_H_BEAM_IRQ_SOURCE);
	}
	break;
    }
    S9xReschedule ();
}
