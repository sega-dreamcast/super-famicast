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
#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#include "snes9x.h"
#include "memmap.h"
#include "display.h"
#include "cheats.h"

#ifdef DEBUGGER
extern FILE *trace;
#endif

void S9xParseCheatsFile (const char *rom_filename)
{
    FILE *f;
    char dir [_MAX_DIR];
    char drive [_MAX_DRIVE];
    char name [_MAX_FNAME];
    char ext [_MAX_EXT];
    char fname [_MAX_PATH];
    char buf [80];
    uint32 address;
    uint8 byte;
    uint8 bytes [3];
    bool8 sram;
    uint8 num_bytes;
    const char *error;
    char *p;

    _splitpath (rom_filename, drive, dir, name, ext);
    _makepath (fname, drive, dir, name, "pat");

    if ((f = fopen(fname, "r")) != NULL)
    {
        while(fgets(buf, 80, f) != NULL)
        {
	    if ((p = strrchr (buf, '\n')) != NULL) 
		*p = '\0';
	    if (((error = S9xGameGenieToRaw (buf, address, byte)) == NULL) ||
		((error = S9xProActionReplayToRaw (buf, address, byte)) == NULL))
	    {
		S9xAddCheat (TRUE, FALSE, address, byte);
	    }
	    else
	    if ((error = S9xGoldFingerToRaw (buf, address, sram,
					     num_bytes, bytes)) == NULL)
	    {
		for (int c = 0; c < num_bytes; c++)
		    S9xAddCheat (TRUE, FALSE, address + c, bytes [c]);
	    }
	    else
		S9xMessage (S9X_ERROR, S9X_GAME_GENIE_CODE_ERROR, error);
        }
        fclose(f);
    }
}
