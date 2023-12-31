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

#ifndef _CPUMACRO_H_
#define _CPUMACRO_H_

STATIC SCHERZO_INLINE void SetZN16 (uint16 Work)
{
    ICPU._Zero = Work != 0;
    ICPU._Negative = (uint8) (Work >> 8);
}

STATIC SCHERZO_INLINE void SetZN8 (uint8 Work)
{
    ICPU._Zero = Work;
    ICPU._Negative = Work;
}

STATIC SCHERZO_INLINE void ADC8 ()
{
    Work8 = S9xGetByte(OpAddress);
    if (CheckDecimal())
    {
		A1 = (Registers.A.W) & 0xF;
		A2 = (Registers.A.W >> 4) & 0xF;
		W1 = Work8 & 0xF;
		W2 = (Work8 >> 4) & 0xF;
		A1 += W1 + CheckCarry();
		if (A1 > 9)
		{
			A1 -= 10;
			A1 &= 0xF;
			A2++;
		}
		A2 += W2;
		if (A2 > 9)
		{
			A2 -= 10;
			A2 &= 0xF;
			SetCarry();
		}
		else
			ClearCarry();
		Ans8 = (A2 << 4) | A1;
		if (~(Registers.AL ^ Work8) & (Work8 ^ Ans8) & 0x80)
	    	SetOverflow();
		else
		    ClearOverflow();
		Registers.AL = Ans8;
		SetZN8(Registers.AL);
    }
    else
    {
		Ans16 = Registers.AL + Work8 + CheckCarry();
		ICPU._Carry = Ans16 >= 0x100;

		if (~(Registers.AL ^ Work8) & (Work8 ^ (uint8) Ans16) & 0x80)
			SetOverflow();
		else
			ClearOverflow();
		Registers.AL = (uint8) Ans16;
		SetZN8(Registers.AL);
    }
}

STATIC SCHERZO_INLINE void ADC16()
{
    Work16 = S9xGetWord(OpAddress);
    if (CheckDecimal ())
    {
		A1 = (Registers.A.W) & 0xF;
		A2 = (Registers.A.W >> 4) & 0xF;
		A3 = (Registers.A.W >> 8) & 0xF;
		A4 = (Registers.A.W >> 12) & 0xF;
		W1 = Work16 & 0xF;
		W2 = (Work16 >> 4) & 0xF;
		W3 = (Work16 >> 8) & 0xF;
		W4 = (Work16 >> 12) & 0xF;
		A1 += W1 + CheckCarry ();
		if (A1 > 9)
		{
			A1 -= 10;
			A1 &= 0xF;
			A2++;
		}
		A2 += W2;
		if (A2 > 9)
		{
			A2 -= 10;
			A2 &= 0xF;
			A3++;
		}
		A3 += W3;
		if (A3 > 9)
		{
			A3 -= 10;
			A3 &= 0xF;
			A4++;
		}
		A4 += W4;
		if (A4 > 9)
		{
			A4 -= 10;
			A4 &= 0xF;
			SetCarry();
		}
		else
			ClearCarry();
		Ans16 = (A4 << 12) | (A3 << 8) | (A2 << 4) | (A1);
		if (~(Registers.A.W ^ Work16) &
			(Work16 ^ Ans16) & 0x8000)
			SetOverflow();
		else
			ClearOverflow();
		Registers.A.W = Ans16;
		SetZN16(Registers.A.W);
    }
    else
    {
		Ans32 = Registers.A.W + Work16 + CheckCarry();
		ICPU._Carry = Ans32 >= 0x10000;
		if (~(Registers.A.W ^ Work16) &
			(Work16 ^ (uint16) Ans32) & 0x8000)
			SetOverflow();
		else
			ClearOverflow();
		Registers.A.W = (uint16) Ans32;
		SetZN16(Registers.A.W);
    }
}

STATIC SCHERZO_INLINE void AND16 ()
{
    Registers.A.W &= S9xGetWord (OpAddress);
    SetZN16 (Registers.A.W);
}

STATIC SCHERZO_INLINE void AND8 ()
{
    Registers.AL &= S9xGetByte (OpAddress);
    SetZN8 (Registers.AL);
}

STATIC SCHERZO_INLINE void A_ASL16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    ICPU._Carry = (Registers.AH & 0x80) != 0;
    Registers.A.W <<= 1;
    SetZN16 (Registers.A.W);
}

STATIC SCHERZO_INLINE void A_ASL8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    ICPU._Carry = (Registers.AL & 0x80) != 0;
    Registers.AL <<= 1;
    SetZN8 (Registers.AL);
}

STATIC SCHERZO_INLINE void ASL16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work16 = S9xGetWord (OpAddress);
    ICPU._Carry = (Work16 & 0x8000) != 0;
    Work16 <<= 1;
    //S9xSetWord (Work16, OpAddress);
	S9xSetByte(Work16>>8, OpAddress+1);
	S9xSetByte(Work16&0xFF, OpAddress);
    SetZN16 (Work16);
}

STATIC SCHERZO_INLINE void ASL8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work8 = S9xGetByte (OpAddress);
    ICPU._Carry = (Work8 & 0x80) != 0;
    Work8 <<= 1;
    S9xSetByte (Work8, OpAddress);
    SetZN8 (Work8);
}

STATIC SCHERZO_INLINE void BIT16 ()
{
    Work16 = S9xGetWord (OpAddress);
    ICPU._Overflow = (Work16 & 0x4000) != 0;
    ICPU._Negative = (uint8) (Work16 >> 8);
    ICPU._Zero = (Work16 & Registers.A.W) != 0;
}

STATIC SCHERZO_INLINE void BIT8 ()
{
    Work8 = S9xGetByte (OpAddress);
    ICPU._Overflow = (Work8 & 0x40) != 0;
    ICPU._Negative = Work8;
    ICPU._Zero = Work8 & Registers.AL;
}

STATIC SCHERZO_INLINE void CMP16 ()
{
    Int32 = (long) Registers.A.W -
	    (long) S9xGetWord (OpAddress);
    ICPU._Carry = Int32 >= 0;
    SetZN16 ((uint16) Int32);
}

STATIC SCHERZO_INLINE void CMP8 ()
{
    Int16 = (short) Registers.AL -
	    (short) S9xGetByte (OpAddress);
    ICPU._Carry = Int16 >= 0;
    SetZN8 ((uint8) Int16);
}

STATIC SCHERZO_INLINE void CMX16 ()
{
    Int32 = (long) Registers.X.W -
	    (long) S9xGetWord (OpAddress);
    ICPU._Carry = Int32 >= 0;
    SetZN16 ((uint16) Int32);
}

STATIC SCHERZO_INLINE void CMX8 ()
{
    Int16 = (short) Registers.XL -
	    (short) S9xGetByte (OpAddress);
    ICPU._Carry = Int16 >= 0;
    SetZN8 ((uint8) Int16);
}

STATIC SCHERZO_INLINE void CMY16 ()
{
    Int32 = (long) Registers.Y.W -
	    (long) S9xGetWord (OpAddress);
    ICPU._Carry = Int32 >= 0;
    SetZN16 ((uint16) Int32);
}

STATIC SCHERZO_INLINE void CMY8 ()
{
    Int16 = (short) Registers.YL -
	    (short) S9xGetByte (OpAddress);
    ICPU._Carry = Int16 >= 0;
    SetZN8 ((uint8) Int16);
}

STATIC SCHERZO_INLINE void A_DEC16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
    CPU.WaitAddress = NULL;
#endif

    Registers.A.W--;
    SetZN16 (Registers.A.W);
}

STATIC SCHERZO_INLINE void A_DEC8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
    CPU.WaitAddress = NULL;
#endif

    Registers.AL--;
    SetZN8 (Registers.AL);
}

STATIC SCHERZO_INLINE void DEC16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
    CPU.WaitAddress = NULL;
#endif

    Work16 = S9xGetWord (OpAddress) - 1;
    //S9xSetWord (Work16, OpAddress);
    S9xSetByte (Work16>>8, OpAddress+1);
	S9xSetByte (Work16&0xFF, OpAddress);
	SetZN16 (Work16);
}

STATIC SCHERZO_INLINE void DEC8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
    CPU.WaitAddress = NULL;
#endif

    Work8 = S9xGetByte (OpAddress) - 1;
    S9xSetByte (Work8, OpAddress);
    SetZN8 (Work8);
}

STATIC SCHERZO_INLINE void EOR16 ()
{
    Registers.A.W ^= S9xGetWord (OpAddress);
    SetZN16 (Registers.A.W);
}

STATIC SCHERZO_INLINE void EOR8 ()
{
    Registers.AL ^= S9xGetByte (OpAddress);
    SetZN8 (Registers.AL);
}

STATIC SCHERZO_INLINE void A_INC16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
    CPU.WaitAddress = NULL;
#endif

    Registers.A.W++;
    SetZN16 (Registers.A.W);
}

STATIC SCHERZO_INLINE void A_INC8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
    CPU.WaitAddress = NULL;
#endif

    Registers.AL++;
    SetZN8 (Registers.AL);
}

STATIC SCHERZO_INLINE void INC16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
    CPU.WaitAddress = NULL;
#endif

    Work16 = S9xGetWord (OpAddress) + 1;
    //S9xSetWord (Work16, OpAddress);
	S9xSetByte (Work16>>8, OpAddress+1);
	S9xSetByte (Work16&0xFF, OpAddress);
    SetZN16 (Work16);
}

STATIC SCHERZO_INLINE void INC8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
#ifdef CPU_SHUTDOWN
    CPU.WaitAddress = NULL;
#endif

    Work8 = S9xGetByte (OpAddress) + 1;
    S9xSetByte (Work8, OpAddress);
    SetZN8 (Work8);
}

STATIC SCHERZO_INLINE void LDA16 ()
{
    Registers.A.W = S9xGetWord (OpAddress);
    SetZN16 (Registers.A.W);
}

STATIC SCHERZO_INLINE void LDA8 ()
{
    Registers.AL = S9xGetByte (OpAddress);
    SetZN8 (Registers.AL);
}

STATIC SCHERZO_INLINE void LDX16 ()
{
    Registers.X.W = S9xGetWord (OpAddress);
    SetZN16 (Registers.X.W);
}

STATIC SCHERZO_INLINE void LDX8 ()
{
    Registers.XL = S9xGetByte (OpAddress);
    SetZN8 (Registers.XL);
}

STATIC SCHERZO_INLINE void LDY16 ()
{
    Registers.Y.W = S9xGetWord (OpAddress);
    SetZN16 (Registers.Y.W);
}

STATIC SCHERZO_INLINE void LDY8 ()
{
    Registers.YL = S9xGetByte (OpAddress);
    SetZN8 (Registers.YL);
}

STATIC SCHERZO_INLINE void A_LSR16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    ICPU._Carry = Registers.AL & 1;
    Registers.A.W >>= 1;
    SetZN16 (Registers.A.W);
}

STATIC SCHERZO_INLINE void A_LSR8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    ICPU._Carry = Registers.AL & 1;
    Registers.AL >>= 1;
    SetZN8 (Registers.AL);
}

STATIC SCHERZO_INLINE void LSR16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work16 = S9xGetWord (OpAddress);
    ICPU._Carry = Work16 & 1;
    Work16 >>= 1;
    //S9xSetWord (Work16, OpAddress);
	S9xSetByte (Work16>>8, OpAddress+1);
	S9xSetByte (Work16&0xFF, OpAddress);
    SetZN16 (Work16);
}

STATIC SCHERZO_INLINE void LSR8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work8 = S9xGetByte (OpAddress);
    ICPU._Carry = Work8 & 1;
    Work8 >>= 1;
    S9xSetByte (Work8, OpAddress);
    SetZN8 (Work8);
}

STATIC SCHERZO_INLINE void ORA16 ()
{
    Registers.A.W |= S9xGetWord (OpAddress);
    SetZN16 (Registers.A.W);
}

STATIC SCHERZO_INLINE void ORA8 ()
{
    Registers.AL |= S9xGetByte (OpAddress);
    SetZN8 (Registers.AL);
}

STATIC SCHERZO_INLINE void A_ROL16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work32 = (Registers.A.W << 1) | CheckCarry();
    ICPU._Carry = Work32 >= 0x10000;
    Registers.A.W = (uint16) Work32;
    SetZN16 ((uint16) Work32);
}

STATIC SCHERZO_INLINE void A_ROL8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work16 = Registers.AL;
    Work16 <<= 1;
    Work16 |= CheckCarry();
    ICPU._Carry = Work16 >= 0x100;
    Registers.AL = (uint8) Work16;
    SetZN8 ((uint8) Work16);
}

STATIC SCHERZO_INLINE void ROL16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work32 = S9xGetWord (OpAddress);
    Work32 <<= 1;
    Work32 |= CheckCarry();
    ICPU._Carry = Work32 >= 0x10000;
    //S9xSetWord ((uint16) Work32, OpAddress);
	S9xSetByte((Work32>>8)&0xFF, OpAddress+1);
	S9xSetByte(Work32&0xFF, OpAddress);
    SetZN16 ((uint16) Work32);
}

STATIC SCHERZO_INLINE void ROL8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work16 = S9xGetByte (OpAddress);
    Work16 <<= 1;
    Work16 |= CheckCarry ();
    ICPU._Carry = Work16 >= 0x100;
    S9xSetByte ((uint8) Work16, OpAddress);
    SetZN8 ((uint8) Work16);
}

STATIC SCHERZO_INLINE void A_ROR16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work32 = Registers.A.W;
    Work32 |= (int) CheckCarry() << 16;
    ICPU._Carry = (uint8) (Work32 & 1);
    Work32 >>= 1;
    Registers.A.W = (uint16) Work32;
    SetZN16 ((uint16) Work32);
}

STATIC SCHERZO_INLINE void A_ROR8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work16 = Registers.AL | ((uint16) CheckCarry() << 8);
    ICPU._Carry = (uint8) Work16 & 1;
    Work16 >>= 1;
    Registers.AL = (uint8) Work16;
    SetZN8 ((uint8) Work16);
}

STATIC SCHERZO_INLINE void ROR16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work32 = S9xGetWord (OpAddress);
    Work32 |= (int) CheckCarry() << 16;
    ICPU._Carry = (uint8) (Work32 & 1);
    Work32 >>= 1;
    //S9xSetWord ((uint16) Work32, OpAddress);
	S9xSetByte ( (Work32>>8)&0x00FF, OpAddress+1);
	S9xSetByte (Work32&0x00FF, OpAddress);
    SetZN16 ((uint16) Work32);
}

STATIC SCHERZO_INLINE void ROR8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work16 = S9xGetByte (OpAddress);
    Work16 |= (int) CheckCarry () << 8;
    ICPU._Carry = (uint8) (Work16 & 1);
    Work16 >>= 1;
    S9xSetByte ((uint8) Work16, OpAddress);
    SetZN8 ((uint8) Work16);
}

STATIC SCHERZO_INLINE void SBC16 ()
{
    Work16 = S9xGetWord (OpAddress);

    if (CheckDecimal ())
    {
	A1 = (Registers.A.W) & 0xF;
	A2 = (Registers.A.W >> 4) & 0xF;
	A3 = (Registers.A.W >> 8) & 0xF;
	A4 = (Registers.A.W >> 12) & 0xF;
	W1 = Work16 & 0xF;
	W2 = (Work16 >> 4) & 0xF;
	W3 = (Work16 >> 8) & 0xF;
	W4 = (Work16 >> 12) & 0xF;

	A1 -= W1 + !CheckCarry ();
	A2 -= W2;
	A3 -= W3;
	A4 -= W4;
	if (A1 > 9)
	{
	    A1 += 10;
	    A2--;
	}
	if (A2 > 9)
	{
	    A2 += 10;
	    A3--;
	}
	if (A3 > 9)
	{
	    A3 += 10;
	    A4--;
	}
	if (A4 > 9)
	{
	    A4 += 10;
	    ClearCarry ();
	}
	else
	{
	    SetCarry ();
	}

	Ans16 = (A4 << 12) | (A3 << 8) | (A2 << 4) | (A1);
	if ((Registers.A.W ^ Work16) &
	    (Registers.A.W ^ Ans16) & 0x8000)
	    SetOverflow();
	else
	    ClearOverflow();
	Registers.A.W = Ans16;
	SetZN16 (Registers.A.W);
    }
    else
    {

	Int32 = (long) Registers.A.W - (long) Work16 + (long) CheckCarry() - 1;

	ICPU._Carry = Int32 >= 0;

	if ((Registers.A.W ^ Work16) &
	    (Registers.A.W ^ (uint16) Int32) & 0x8000)
	    SetOverflow();
	else
	    ClearOverflow ();
	Registers.A.W = (uint16) Int32;
	SetZN16 (Registers.A.W);
    }
}

STATIC SCHERZO_INLINE void SBC8 ()
{
    Work8 = S9xGetByte (OpAddress);
    if (CheckDecimal ())
    {
	A1 = (Registers.A.W) & 0xF;
	A2 = (Registers.A.W >> 4) & 0xF;
	W1 = Work8 & 0xF;
	W2 = (Work8 >> 4) & 0xF;

	A1 -= W1 + !CheckCarry ();
	A2 -= W2;
	if (A1 > 9)
	{
	    A1 += 10;
	    A2--;
	}
	if (A2 > 9)
	{
	    A2 += 10;
	    ClearCarry ();
	}
	else
	{
	    SetCarry ();
	}

	Ans8 = (A2 << 4) | A1;
	if ((Registers.AL ^ Work8) &
	    (Registers.AL ^ Ans8) & 0x80)
	    SetOverflow ();
	else
	    ClearOverflow ();
	Registers.AL = Ans8;
	SetZN8 (Registers.AL);
    }
    else
    {
	Int16 = (short) Registers.AL - (short) Work8 + (short) CheckCarry() - 1;

	ICPU._Carry = Int16 >= 0;
	if ((Registers.AL ^ Work8) &
	    (Registers.AL ^ (uint8) Int16) & 0x80)
	    SetOverflow ();
	else
	    ClearOverflow ();
	Registers.AL = (uint8) Int16;
	SetZN8 (Registers.AL);
    }
}

STATIC SCHERZO_INLINE void STA16 ()
{
    S9xSetWord (Registers.A.W, OpAddress);
}

STATIC SCHERZO_INLINE void STA8 ()
{
    S9xSetByte (Registers.AL, OpAddress);
}

STATIC SCHERZO_INLINE void STX16 ()
{
    S9xSetWord (Registers.X.W, OpAddress);
}

STATIC SCHERZO_INLINE void STX8 ()
{
    S9xSetByte (Registers.XL, OpAddress);
}

STATIC SCHERZO_INLINE void STY16 ()
{
    S9xSetWord (Registers.Y.W, OpAddress);
}

STATIC SCHERZO_INLINE void STY8 ()
{
    S9xSetByte (Registers.YL, OpAddress);
}

STATIC SCHERZO_INLINE void STZ16 ()
{
    S9xSetWord (0, OpAddress);
}

STATIC SCHERZO_INLINE void STZ8 ()
{
    S9xSetByte (0, OpAddress);
}

STATIC SCHERZO_INLINE void TSB16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work16 = S9xGetWord (OpAddress);
    ICPU._Zero = (Work16 & Registers.A.W) != 0;
    Work16 |= Registers.A.W;
    //S9xSetWord (Work16, OpAddress);
	S9xSetByte (Work16>>8, OpAddress+1);
	S9xSetByte (Work16&0xFF, OpAddress);
}

STATIC SCHERZO_INLINE void TSB8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work8 = S9xGetByte (OpAddress);
    ICPU._Zero = Work8 & Registers.AL;
    Work8 |= Registers.AL;
    S9xSetByte (Work8, OpAddress);
}

STATIC SCHERZO_INLINE void TRB16 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work16 = S9xGetWord (OpAddress);
    ICPU._Zero = (Work16 & Registers.A.W) != 0;
    Work16 &= ~Registers.A.W;
    //S9xSetWord (Work16, OpAddress);
	S9xSetByte (Work16>>8, OpAddress+1);
	S9xSetByte (Work16&0xFF, OpAddress);
}

STATIC SCHERZO_INLINE void TRB8 ()
{
#ifndef SA1_OPCODES
    CPU.Cycles += ONE_CYCLE;
#endif
    Work8 = S9xGetByte (OpAddress);
    ICPU._Zero = Work8 & Registers.AL;
    Work8 &= ~Registers.AL;
    S9xSetByte (Work8, OpAddress);
}
#endif
