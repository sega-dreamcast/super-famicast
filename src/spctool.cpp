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
#include "spctool/dsp.h"
#include "spctool/spc700.h"
#include "spctool/soundmod.h"
#include "apu.h"

bool8 S9xOpenSoundDevice (int, int);

void S9xSetPlaybackRate (uint32 rate)
{
    DOpt		SmpOpt;
    
    SmpOpt.Smp8bit=false;
    SmpOpt.SmpMono=false;
    SmpOpt.IntType=IntC;
    SmpOpt.OldBRE=false;
    SmpOpt.MixRout=1;
    SetSPUOpt (rate, SmpOpt);

//    so.playback_rate = playback_rate;
//    so.err_rate = (uint32) (SNES_SCANLINE_TIME * FIXED_POINT / (1.0 / (double) so.playback_rate));
 }

bool8 S9xSetSoundMute (bool8 mute)
{
    return (TRUE);
}

START_EXTERN_C
bool8 S9xInitSound (int mode, int buffer_size)
{
    if (!(mode & 7))
		return (TRUE);

    S9xSetSoundMute (TRUE);
    if (!S9xOpenSoundDevice (mode, buffer_size))
    {
		S9xMessage (S9X_ERROR, S9X_SOUND_DEVICE_OPEN_FAILED, "Sound device open failed");
		return (FALSE);
    }
    return (TRUE);
}

void TraceSPC (unsigned char *PC, unsigned short YA, unsigned char X,
	       SPCFlags PS, unsigned char *SP)
{
    APURegisters.YA.W = YA;
    APURegisters.X = X;
    APURegisters.S = SP - IAPU.RAM;
    IAPU.PC = PC;
    IAPU._Carry = PS.C;
    IAPU._Zero = !PS.Z | (PS.N << 7);
    IAPU._Overflow = PS.V;
    APURegisters.P = *(uint8 *) &PS;
    S9xTraceAPU ();
}

bool8 S9xInitAPU ()
{
    void		*SPCBase;               //Base pointer and aligned pointer to SPC RAM
    
    SPCBase=malloc(131072);                  //Allocate memory for SPC RAM

    IAPU.RAM=(uint8 *) InitSPU(SPCBase);                 //Initialize SPU w/ ptr to SPC RAM (Call only once)

    S9xSetPlaybackRate (22000);
    ResetSPU(20);                            //Reset SPU with pre-amp level of 30
//    _SetSPCDbg(TraceSPC);                   //Install debug handler
   return (TRUE);
}

void S9xResetAPU ()
{
    ResetSPU(20);
    IAPU.RAM [0xf1] = 0x80;
    _FixSPC (0xffc0, 0, 0, 0, 0, 0xff);
//    FixDSP ();
}

extern "C" void EDSP (uint8 *, int32);

void S9xMixSamplesO (uint8 *buffer, int sample_count, int byte_offset)
{
//    _EmuDSP (buffer + byte_offset, sample_count / 2);

    EDSP (buffer + byte_offset, sample_count / 2);
#if 0
for (int i = 0; i < 32; i++)
printf ("%02x,", *(buffer + byte_offset + i));
printf ("\n");
#endif
}
END_EXTERN_C

void S9xFixSoundAfterSnapshotLoad ()
{
}

void S9xSetSoundControl (int)
{
}

#ifdef DEBUGGER
START_EXTERN_C
void S9xDeinitAPU ()
{
}

END_EXTERN_C
#endif
