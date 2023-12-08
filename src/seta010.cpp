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
#include "seta.h"
#include "memmap.h"

//temporary Op04 requirement
#include <math.h>

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

ST010_Regs ST010;

const short SETA_SinTable[256] = {
         0,    804,   1608,   2410,   3212,   4011,   4808,   5602,
      6393,   7179,   7962,   8739,   9512,  10278,  11039,  11793,
     12539,  13279,  14010,  14732,  15446,  16151,  16846,  17530,
     18204,  18868,  19519,  20159,  20787,  21403,  22005,  22594,
     23170,  23731,  24279,  24811,  25329,  25832,  26319,  26790,
     27245,  27683,  28105,  28510,  28898,  29268,  29621,  29956,
     30273,  30571,  30852,  31113,  31356,  31580,  31785,  31971,
     32137,  32285,  32412,  32521,  32609,  32678,  32728,  32757,
     32767,  32757,  32728,  32678,  32609,  32521,  32412,  32285,
     32137,  31971,  31785,  31580,  31356,  31113,  30852,  30571,
     30273,  29956,  29621,  29268,  28898,  28510,  28105,  27683,
     27245,  26790,  26319,  25832,  25329,  24811,  24279,  23731,
     23170,  22594,  22005,  21403,  20787,  20159,  19519,  18868,
     18204,  17530,  16846,  16151,  15446,  14732,  14010,  13279,
     12539,  11793,  11039,  10278,   9512,   8739,   7962,   7179,
      6392,   5602,   4808,   4011,   3212,   2410,   1608,    804,
         0,   -804,  -1608,  -2411,  -3212,  -4011,  -4808,  -5602,
     -6393,  -7179,  -7962,  -8739,  -9512, -10278, -11039, -11793,
    -12539, -13279, -14010, -14733, -15446, -16151, -16846, -17530,
    -18204, -18868, -19519, -20159, -20787, -21403, -22005, -22594,
    -23170, -23731, -24279, -24812, -25329, -25832, -26319, -26790,
    -27245, -27683, -28105, -28510, -28898, -29268, -29621, -29956,
    -30273, -30571, -30852, -31113, -31356, -31580, -31785, -31971,
    -32137, -32285, -32412, -32521, -32609, -32678, -32728, -32757,
    -32767, -32757, -32728, -32678, -32609, -32521, -32412, -32285,
    -32137, -31971, -31785, -31580, -31356, -31113, -30851, -30571,
    -30273, -29956, -29621, -29268, -28898, -28510, -28105, -27683,
    -27245, -26790, -26319, -25832, -25329, -24811, -24279, -23731,
    -23170, -22594, -22005, -21402, -20787, -20159, -19519, -18867,
    -18204, -17530, -16845, -16151, -15446, -14732, -14009, -13278,
    -12539, -11792, -11039, -10278,  -9511,  -8739,  -7961,  -7179,
     -6392,  -5602,  -4808,  -4011,  -3211,  -2410,  -1607,   -804
};

const char Op01_table [32][32] = {
    { -128,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,
       -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64,  -64, },
    { -128,  -96,  -83,  -77,  -74,  -72,  -71,  -70,  -69,  -69,  -68,  -68,  -67,  -67,  -67,  -67,
       -67,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -65,  -65,  -65,  -65, },
    { -128, -109,  -96,  -88,  -83,  -80,  -77,  -75,  -74,  -73,  -72,  -71,  -71,  -70,  -70,  -69,
       -69,  -69,  -69,  -68,  -68,  -68,  -68,  -68,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67, },
    { -128, -115, -104,  -96,  -90,  -86,  -83,  -80,  -79,  -77,  -76,  -75,  -74,  -73,  -73,  -72,
       -72,  -71,  -71,  -70,  -70,  -70,  -70,  -69,  -69,  -69,  -69,  -69,  -68,  -68,  -68,  -68, },
    { -128, -118, -109, -102,  -96,  -91,  -88,  -85,  -83,  -81,  -80,  -78,  -77,  -76,  -75,  -75,
       -74,  -73,  -73,  -72,  -72,  -72,  -71,  -71,  -71,  -70,  -70,  -70,  -70,  -70,  -69,  -69, },
    { -128, -120, -112, -106, -101,  -96,  -92,  -89,  -87,  -85,  -83,  -81,  -80,  -79,  -78,  -77,
       -76,  -76,  -75,  -74,  -74,  -74,  -73,  -73,  -72,  -72,  -72,  -71,  -71,  -71,  -71,  -71, },
    { -128, -121, -115, -109, -104, -100,  -96,  -93,  -90,  -88,  -86,  -84,  -83,  -82,  -80,  -80,
       -79,  -78,  -77,  -76,  -76,  -75,  -75,  -74,  -74,  -74,  -73,  -73,  -73,  -72,  -72,  -72, },
    { -128, -122, -117, -112, -107, -103,  -99,  -96,  -93,  -91,  -89,  -87,  -86,  -84,  -83,  -82,
       -81,  -80,  -79,  -78,  -78,  -77,  -77,  -76,  -76,  -75,  -75,  -74,  -74,  -74,  -73,  -73, },
    { -128, -123, -118, -113, -109, -105, -102,  -99,  -96,  -94,  -91,  -90,  -88,  -86,  -85,  -84,
       -83,  -82,  -81,  -80,  -80,  -79,  -78,  -78,  -77,  -77,  -76,  -76,  -75,  -75,  -75,  -75, },
    { -128, -123, -119, -115, -111, -107, -104, -101,  -98,  -96,  -96,  -92,  -90,  -89,  -87,  -86,
       -85,  -84,  -83,  -82,  -81,  -80,  -80,  -79,  -79,  -78,  -78,  -77,  -77,  -76,  -76,  -76, },
    { -128, -124, -120, -116, -112, -109, -106, -103, -101,  -98,  -96,  -94,  -92,  -91,  -89,  -88,
       -87,  -86,  -85,  -84,  -83,  -82,  -81,  -81,  -80,  -80,  -79,  -78,  -78,  -78,  -77,  -77, },
    { -128, -124, -121, -117, -114, -111, -108, -105, -102, -100,  -98,  -96,  -94,  -93,  -91,  -90,
       -89,  -87,  -86,  -85,  -84,  -84,  -83,  -82,  -82,  -81,  -80,  -80,  -79,  -79,  -78,  -78, },
    { -128, -125, -121, -118, -115, -112, -109, -106, -104, -102, -100,  -98,  -96,  -94,  -93,  -91,
       -90,  -89,  -88,  -87,  -86,  -85,  -84,  -84,  -83,  -82,  -82,  -81,  -80,  -80,  -80,  -79, },
    { -128, -125, -122, -119, -116, -113, -110, -108, -106, -103, -101,  -99,  -98,  -96,  -94,  -93,
       -92,  -91,  -89,  -88,  -87,  -87,  -86,  -85,  -84,  -84,  -83,  -82,  -82,  -81,  -81,  -80, },
    { -128, -125, -122, -119, -117, -114, -112, -109, -107, -105, -103, -101,  -99,  -98,  -96,  -95,
       -93,  -92,  -91,  -90,  -89,  -88,  -87,  -86,  -86,  -85,  -84,  -83,  -83,  -82,  -82,  -81, },
    { -128, -125, -123, -120, -117, -115, -112, -110, -108, -106, -104, -102, -101,  -99,  -97,  -96,
       -95,  -93,  -92,  -91,  -90,  -89,  -88,  -88,  -87,  -86,  -85,  -85,  -84,  -83,  -83,  -82, },
    { -128, -125, -123, -120, -118, -116, -113, -111, -109, -107, -105, -103, -102, -100,  -99,  -97,
       -96,  -95,  -94,  -93,  -91,  -91,  -90,  -89,  -88,  -87,  -86,  -86,  -85,  -85,  -84,  -83, },
    { -128, -126, -123, -121, -119, -116, -114, -112, -110, -108, -106, -105, -103, -101, -100,  -99,
       -97,  -96,  -95,  -94,  -93,  -92,  -91,  -90,  -89,  -88,  -88,  -87,  -86,  -86,  -85,  -84, },
    { -128, -126, -123, -121, -119, -117, -115, -113, -111, -109, -107, -106, -104, -103, -101, -100,
       -98,  -97,  -96,  -95,  -94,  -93,  -92,  -91,  -90,  -89,  -89,  -88,  -87,  -87,  -86,  -85, },
    { -128, -126, -124, -122, -120, -118, -116, -114, -112, -110, -108, -107, -105, -104, -102, -101,
       -99,  -98,  -97,  -96,  -95,  -94,  -93,  -92,  -91,  -90,  -90,  -89,  -88,  -88,  -87,  -86, },
    { -128, -126, -124, -122, -120, -118, -116, -114, -112, -111, -109, -108, -106, -105, -103, -102,
      -101,  -99,  -98,  -97,  -96,  -95,  -94,  -93,  -92,  -91,  -91,  -90,  -89,  -89,  -88,  -87, },
    { -128, -126, -124, -122, -120, -118, -117, -115, -113, -112, -110, -108, -107, -105, -104, -103,
      -101, -100,  -99,  -98,  -97,  -96,  -95,  -94,  -93,  -92,  -92,  -91,  -90,  -90,  -89,  -88, },
    { -128, -126, -124, -122, -121, -119, -117, -115, -114, -112, -111, -109, -108, -106, -105, -104,
      -102, -101, -100,  -99,  -98,  -97,  -96,  -95,  -94,  -93,  -93,  -92,  -91,  -90,  -90,  -89, },
    { -128, -126, -124, -123, -121, -119, -118, -116, -114, -113, -111, -110, -108, -107, -106, -104,
      -103, -102, -101, -100,  -99,  -98,  -97,  -96,  -95,  -94,  -94,  -93,  -92,  -91,  -91,  -90, },
    { -128, -126, -125, -123, -121, -120, -118, -116, -115, -113, -112, -110, -109, -108, -106, -105,
      -104, -103, -102, -101, -100,  -99,  -98,  -97,  -96,  -95,  -94,  -94,  -93,  -92,  -91,  -91, },
    { -128, -126, -125, -123, -122, -120, -118, -117, -115, -114, -112, -111, -110, -108, -107, -106,
      -105, -104, -103, -102, -101, -100,  -99,  -98,  -97,  -96,  -95,  -94,  -94,  -93,  -92,  -92, },
    { -128, -126, -125, -123, -122, -120, -119, -117, -116, -114, -113, -112, -110, -109, -108, -107,
      -106, -104, -103, -102, -101, -100,  -99,  -98,  -98,  -97,  -96,  -95,  -94,  -94,  -93,  -92, },
    { -128, -126, -125, -123, -122, -121, -119, -118, -116, -115, -114, -112, -111, -110, -109, -107,
      -106, -105, -104, -103, -102, -101, -100, -99,   -98,  -98,  -97,  -96,  -95,  -95,  -94,  -93, },
    { -128, -127, -125, -124, -122, -121, -119, -118, -117, -115, -114, -113, -112, -110, -109, -108,
      -107, -106, -105, -104, -103, -102, -101, -100,  -99,  -98,  -98,  -97,  -96,  -95,  -95,  -94, },
    { -128, -127, -125, -124, -122, -121, -120, -118, -117, -116, -114, -113, -112, -111, -110, -109,
      -107, -106, -105, -104, -103, -102, -102, -101, -100,  -99,  -98,  -97,  -97,  -96,  -95,  -95, },
    { -128, -127, -125, -124, -123, -121, -120, -119, -117, -116, -115, -114, -112, -111, -110, -109,
      -108, -107, -106, -105, -104, -103, -102, -101, -101, -100,  -99,  -98,  -97,  -97,  -96,  -95, },
    { -128, -127, -125, -124, -123, -121, -120, -119, -118, -116, -115, -114, -113, -112, -111, -110,
      -109, -108, -107, -106, -105, -104, -103, -102, -101, -100, -100,  -99,  -98,  -97,  -97,  -96  }
};

short SETA_Sin(short Theta)
{
	return SETA_SinTable[(Theta >> 8) & 0xff];
}

short SETA_Cos(short Theta)
{
	return SETA_SinTable[((Theta + 0x4000) >> 8) & 0xff];
}

uint8 S9xGetST010(uint32 Address)
{
	printf("Read %06X\n", Address);
		return Memory.SRAM[Address&0xFFFF];
}

/*
 *
 Inputs:
 *
 *   scale value: 680004-680005
 *   x:           680002-680003
 *   y:           680000-680001
 *
 * Outputs:
 *
 *   x_scaled: 680014-680017
 *   y_scaled: 680010-680013
 */
void St010_Op03(int16 s, int16 x, int16 y, int32 &xs, int32 &ys )
{
	xs = (int32) x * (int32) s * 2;
	ys = (int32) y * (int32) s * 2;
	return;
}

void St010_Rotate(short Theta, short X0, short Y0, short &X1, short &Y1)
{
	X1 =(short)( ((sin(((short)Theta) * ((float)PI/0x8000))*Y0)*2)  +  ((cos(((short)Theta) * ((float)PI/0x8000))*X0)*2));
	Y1 =(short)( ((cos(((short)Theta) * ((float)PI/0x8000))*Y0)*2) - 	((sin(((short)Theta) * ((float)PI/0x8000))*X0)*2));
}



void S9xSetST010(uint32 Address, uint8 Byte)
{
	if(Address==0x600000)
	{
		ST010.sram_enable=Byte;
		return;
	}
	printf("Write %06X:%02X\n", Address, Byte);

	Memory.SRAM[Address&0xFFFF]=Byte;

	if(Memory.SRAM[0x21]&0x80)
	{
		switch(Memory.SRAM[0x20])
		{
		case 0x06:
			{
				int32 temp=(int16)(Memory.SRAM[0]|(Memory.SRAM[1]<<8));
				temp*=(int16)(Memory.SRAM[2]|(Memory.SRAM[3]<<8));
				temp*=2;
				Memory.SRAM[0x10]=(uint8)(temp);
				Memory.SRAM[0x11]=(uint8)(temp>>8);
				Memory.SRAM[0x12]=(uint8)(temp>>16);
				Memory.SRAM[0x13]=(uint8)(temp>>24);
				break;
			}
		case 0x08:
			{
				int16 angle, xin, xout, yin, yout;
#ifdef FAST_LSB_WORD_ACCESS
				angle=*((int16*)&Memory.SRAM[4]);
				xin=*((int16*)&Memory.SRAM[2]);
				yin=*((int16*)Memory.SRAM);
#else
				angle=Memory.SRAM[4]|(Memory.SRAM[5]<<8);
				xin=Memory.SRAM[2]|(Memory.SRAM[3]<<8);
				yin=Memory.SRAM[0]|(Memory.SRAM[1]<<8);
#endif
				St010_Rotate(angle,xin,yin,xout, yout);

#ifdef FAST_LSB_WORD_ACCESS
				*((int16*)&Memory.SRAM[0x10])=yout;
				*((int16*)&Memory.SRAM[0x12])=xout;
#else
				Memory.SRAM[0x10]=(uint8)yout;
				Memory.SRAM[0x11]=(uint8)(yout>>8);
				Memory.SRAM[0x12]=(uint8)(xout);
				Memory.SRAM[0x13]=(uint8)(xout>>8);
#endif

				//hack to get approximately correct results from the SETA chip
				break;
			}

		case 0x01:
			{
				short a0, b0, a1, b1, c1, d1, o1;
#ifdef FAST_LSB_WORD_ACCESS
				a0=*((short*)Memory.SRAM);
				b0=*((short*)(Memory.SRAM+2));
#else
				a0=Memory.SRAM[0]|(Memory.SRAM[1]<<8);
				b0=Memory.SRAM[2]|(Memory.SRAM[3]<<8);
#endif
	a1 = a0;	
	b1 = b0;
	c1 = 0x0000;
	d1 = b0;

 	if ((a1 < 0) && (b1 < 0))
	{	
		a1 = -a0;
		b1 = -b0;
		c1 = -0x8000;
	}

 	if (a1 < 0)
	{
		a1 = b0;
		b1 = -a0;
		c1 = -0x4000;
	}

 	if (b1 < 0)
	{
		a1 = -b0;
		b1 = a0;
		c1 = 0x4000;
	}

 	while ((a1 > 0x1f) || (b1 > 0x1f))
	{
		if (a1 > 1) a1 >>= 1;
		if (b1 > 1) b1 >>= 1;
	}

	o1 = (Op01_table[b1][a1] << 8) ^ c1;
#ifdef FAST_LSB_WORD_ACCESS
				*((short*)(Memory.SRAM+0x10))=o1;
				*((short*)(Memory.SRAM))=a1;
				*((short*)(Memory.SRAM+2))=b1;
				*((short*)(Memory.SRAM+4))=c1;
				*((short*)(Memory.SRAM+6))=d1;
#else
				Memory.SRAM[0]=a1&0xFF;
				Memory.SRAM[1]=a1>>8;
				Memory.SRAM[2]=b1&0xFF;
				Memory.SRAM[3]=b1>>8;
				Memory.SRAM[4]=c1&0xFF;
				Memory.SRAM[5]=c1>>8;
				Memory.SRAM[6]=d1&0xFF;
				Memory.SRAM[7]=d1>>8;
				Memory.SRAM[0x10]=o1&0xFF;
				Memory.SRAM[0x11]=o1>>8;
#endif

}


		case 0x03:
			{
				int32 outx, outy;
				St010_Op03(Memory.SRAM[4]|(Memory.SRAM[5]<<8),Memory.SRAM[2]|(Memory.SRAM[3]<<8),Memory.SRAM[0]|(Memory.SRAM[1]<<8),outx, outy);
#ifdef FAST_LSB_WORD_ACCESS
				*((int32*)&Memory.SRAM[0x14])=outx;
				*((int32*)&Memory.SRAM[0x10])=outy;
#else
				Memory.SRAM[0x14]=(uint8)outx;
				Memory.SRAM[0x15]=(uint8)(outx>>8);
				Memory.SRAM[0x16]=(uint8)(outx>>16);
				Memory.SRAM[0x17]=(uint8)(outx>>24);

				Memory.SRAM[0x10]=(uint8)outy;
				Memory.SRAM[0x11]=(uint8)(outy>>8);
				Memory.SRAM[0x12]=(uint8)(outy>>16);
				Memory.SRAM[0x13]=(uint8)(outy>>24);
#endif
			}
			break;
		case 0x04:
			{
				int16 square, x,y;
#ifdef FAST_LSB_WORD_ACCESS
				x=*((int16*)Memory.SRAM);
				y=*((int16*)&Memory.SRAM[2]);
#else
				x=Memory.SRAM[0]|(Memory.SRAM[1]<<8);
				y=Memory.SRAM[2]|(Memory.SRAM[3]<<8);
#endif
				square=(int16)sqrt(x*x+y*y);

#ifdef FAST_LSB_WORD_ACCESS
				*((int32*)&Memory.SRAM[0x10])=square;
#else
				Memory.SRAM[0x10]=(uint8)square;
				Memory.SRAM[0x11]=(uint8)(square>>8);
#endif
				break;
			}
		case 0x07:
		case 0x02:
			break;
		default: printf("Unknown Op\n");break;
		}
		Memory.SRAM[0x20]=0;
		Memory.SRAM[0x21]=0;
	}
}
