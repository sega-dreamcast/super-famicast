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
#include "65c816.h"
#include "memmap.h"
#include "ppu.h"
#include "apu.h"
#include "cpuexec.h"
#include "sa1.h"

#ifndef S9xSTREAM
#define S9xSTREAM stdout
#endif

/*
#define OFFSET(N,F) \
fprintf (S9xSTREAM, "#define " #N " CPU + %d\n", (int) &((struct SCPUState *) 0)->F);
#define OFFSET2(N,F) \
fprintf (S9xSTREAM, "#define " #N " Registers + %d\n", (int) &((struct SRegisters *) 0)->F);
#define OFFSET3(F) \
fprintf (S9xSTREAM, "#define " #F " Memory + %d\n", (int) &((class CMemory *) 0)->F);
#define OFFSET4(N,F) \
fprintf (S9xSTREAM, "#define " #N " APU + %d\n", (int) &((struct SAPU *) 0)->F);
#define OFFSET5(N,F) \
fprintf (S9xSTREAM, "#define " #N " IAPU + %d\n", (int) &((struct SIAPU *) 0)->F);
#define OFFSET6(N,F) \
fprintf (S9xSTREAM, "#define " #N " ICPU + %d\n", (int) &((struct SICPU *) 0)->F);
#define OFFSET7(N,F) \
fprintf (S9xSTREAM, "#define " #N " Settings + %d\n", (int) &((struct SSettings *) 0)->F);
#define OFFSET8(N, F) \
fprintf (S9xSTREAM, "#define " #N " APURegisters + %d\n", (int) &((struct SAPURegisters *) 0)->F);

#define OFFSET9(N, F) \
fprintf (S9xSTREAM, "#define " #N " PPU + %d\n", (int) &((struct SPPU *) 0)->F);
#define OFFSET10(N, F) \
fprintf (S9xSTREAM, "#define " #N " IPPU + %d\n", (int) &((struct InternalPPU *) 0)->F);
#define OFFSET11(N, F) \
fprintf (S9xSTREAM, "#define " #N " SA1 + %d\n", (int) &((struct SSA1 *) 0)->F);
#define OFFSET12(N, F) \
fprintf (S9xSTREAM, "#define " #N " SA1Registers + %d\n", (int) &((struct SSA1Registers *) 0)->F);
*/

#define OFFSET(Name, Reg, S, Member) \
fprintf (S9xSTREAM, "#define " #Name " @(%d," #Reg ")\n", (int) &((struct S *) 0)->Member);

#define OFFSET_OFFSET(Name, Reg, S, Member, off) \
fprintf (S9xSTREAM, "#define " #Name " @(%d," #Reg ")\n", ((int) &((struct S *) 0)->Member) + off);

#define OFFSET_MEMORY(F) \
fprintf (S9xSTREAM, "#define " #F " _Memory + %d\n", (int) &((class CMemory *) 0)->F);

#define JUST_THE_NUMBER(Name, S, Member) \
fprintf (S9xSTREAM, "#define " #Name " %d\n", (int) &((class S *) 0)->Member);

void PrintOffsetsAsm()
{
	fprintf (S9xSTREAM, "START OFFSETS\n");
	
	//SCPUState
	fprintf (S9xSTREAM, "\n//SCPUState\n");
	OFFSET(Flags, r12, SCPUState, Flags)
	OFFSET(BranchSkip, r12, SCPUState, BranchSkip)
	OFFSET(NMIActive, r12, SCPUState, NMIActive)
	OFFSET(IRQActive, r12, SCPUState, IRQActive)
	OFFSET(WaitingForInterrupt, r12, SCPUState, WaitingForInterrupt)
	OFFSET(InDMA, r12, SCPUState, InDMA)
	OFFSET(WhichEvent, r12, SCPUState, WhichEvent)
	OFFSET(PCS, r12, SCPUState, PC)
	OFFSET(PCBase, r12, SCPUState, PCBase)
	OFFSET(PCAtOpcodeStart, r12, SCPUState, PCAtOpcodeStart)
	OFFSET(WaitAddress, r12, SCPUState, WaitAddress)
	OFFSET(WaitCounter, r12, SCPUState, WaitCounter)
	OFFSET(Cycles, r12, SCPUState, Cycles)
	OFFSET(NextEvent, r12, SCPUState, NextEvent)
	OFFSET(V_Counter, r12, SCPUState, V_Counter)
	OFFSET(MemSpeed, r12, SCPUState, MemSpeed)
	OFFSET(MemSpeedx2, r12, SCPUState, MemSpeedx2)
	OFFSET(FastROMSpeed, r12, SCPUState, FastROMSpeed)
	OFFSET(AutoSaveTimer, r12, SCPUState, AutoSaveTimer)
	OFFSET(SRAMModified, r12, SCPUState, SRAMModified)
	OFFSET(NMITriggerPoint, r12, SCPUState, NMITriggerPoint)
	OFFSET(TriedInterleavedMode2, r12, SCPUState, TriedInterleavedMode2)
	OFFSET(BRKTriggered, r12, SCPUState, BRKTriggered)
	OFFSET(NMICycleCount, r12, SCPUState, NMICycleCount)
	OFFSET(IRQCycleCount, r12, SCPUState, IRQCycleCount)
	
	//SRegisters
	fprintf (S9xSTREAM, "\n//SRegisters\n");
	OFFSET(PB, r11, SRegisters, PB)
	OFFSET(DB, r11, SRegisters, DB)
	OFFSET(PP, r11, SRegisters, P.W)
	OFFSET(PL, r11, SRegisters, P.W)
	OFFSET_OFFSET(PH, r11, SRegisters, P.W, 1)
	OFFSET(AA, r11, SRegisters, A.W)
	OFFSET(AL, r11, SRegisters, A.W)
	OFFSET_OFFSET(AH, r11, SRegisters, A.W, 1)
	OFFSET(DD, r11, SRegisters, D.W)
	OFFSET(DL, r11, SRegisters, D.W)
	OFFSET_OFFSET(DH, r11, SRegisters, D.W, 1)
	OFFSET(SS, r11, SRegisters, S.W)
	OFFSET(SL, r11, SRegisters, S.W)
	OFFSET_OFFSET(SH, r11, SRegisters, S.W, 1)
	OFFSET(XX, r11, SRegisters, X.W)
	OFFSET(XL, r11, SRegisters, X.W)
	OFFSET_OFFSET(XH, r11, SRegisters, X.W, 1)
	OFFSET(YY, r11, SRegisters, Y.W)
	OFFSET(YL, r11, SRegisters, Y.W)
	OFFSET_OFFSET(YH, r11, SRegisters, Y.W, 1)
	OFFSET(PCR, r11, SRegisters, PC)
	
	//CMemory
	fprintf (S9xSTREAM, "\n//CMemory\n");
	OFFSET_MEMORY(RAM)
	OFFSET_MEMORY(ROM)
	OFFSET_MEMORY(VRAM)
	OFFSET_MEMORY(SRAM)
	OFFSET_MEMORY(BWRAM)
	OFFSET_MEMORY(FillRAM)
	OFFSET_MEMORY(C4RAM)
	OFFSET_MEMORY(HiROM)
	OFFSET_MEMORY(LoROM)
	OFFSET_MEMORY(SRAMMask)
	OFFSET_MEMORY(SRAMSize)
	OFFSET_MEMORY(Map)
	OFFSET_MEMORY(WriteMap)
	OFFSET_MEMORY(MemorySpeed)
	OFFSET_MEMORY(BlockIsRAM)
	OFFSET_MEMORY(BlockIsROM)
	OFFSET_MEMORY(ROMFilename)
	
	//SIAPU
	fprintf (S9xSTREAM, "\n//SIAPU\n");
	OFFSET(APUPCS, r1, SIAPU, PC)
	OFFSET(APURAM, r1, SIAPU,RAM)
	OFFSET(APUExecuting, r1, SIAPU,APUExecuting)
	OFFSET(APUDirectPage, r1, SIAPU,DirectPage)
	OFFSET(APUBit, r1, SIAPU,Bit)
	OFFSET(APUAddress, r1, SIAPU,Address)
	OFFSET(APUWaitAddress1, r1, SIAPU,WaitAddress1)
	OFFSET(APUWaitAddress2, r1, SIAPU,WaitAddress2)
	OFFSET(APUWaitCounter, r1, SIAPU,WaitCounter)
	OFFSET(APUShadowRAM, r1, SIAPU,ShadowRAM)
	OFFSET(APUCachedSamples, r1, SIAPU,CachedSamples)
	OFFSET(APU_Carry, r1, SIAPU,_Carry)
	OFFSET(APU_Zero, r1, SIAPU,_Zero)
	OFFSET(APU_Overflow, r1, SIAPU,_Overflow)
	OFFSET(APUTimerErrorCounter, r1, SIAPU,TimerErrorCounter)

	//SAPU
	fprintf (S9xSTREAM, "\n//SAPU\n");
	OFFSET(APUCycles, r1, SAPU,Cycles)
	OFFSET(APUShowROM, r1, SAPU,ShowROM)
	OFFSET(APUFlags, r1, SAPU,Flags)
	OFFSET(APUKeyedChannels, r1, SAPU,KeyedChannels)
	OFFSET(APUOutPorts, r1, SAPU,OutPorts)
	OFFSET(APUDSP, r1, SAPU,DSP)
	
	JUST_THE_NUMBER(APUExtraRAM, SAPU,ExtraRAM)
	JUST_THE_NUMBER(APUTimer, SAPU,Timer)
	JUST_THE_NUMBER(APUTimerTarget, SAPU,TimerTarget)
	JUST_THE_NUMBER(APUTimerEnabled, SAPU,TimerEnabled)
	JUST_THE_NUMBER(TimerValueWritten, SAPU,TimerValueWritten)

	//SICPU
	fprintf (S9xSTREAM, "\n//SICPU\n");
	OFFSET(CPUSpeed, r1, SICPU,Speed)
	OFFSET(CPUOpcodes, r1, SICPU,S9xOpcodes)
	OFFSET(_Carry, r1, SICPU,_Carry)
	OFFSET(_Zero, r1, SICPU,_Zero)
	OFFSET(_Negative, r1, SICPU,_Negative)
	OFFSET(_Overflow, r1, SICPU,_Overflow)
	OFFSET(ShiftedDB, r1, SICPU,ShiftedDB)
	OFFSET(ShiftedPB, r1, SICPU,ShiftedPB)
	OFFSET(CPUExecuting, r1, SICPU,CPUExecuting)
	OFFSET(Scanline, r1, SICPU,Scanline)
	OFFSET(Frame, r1, SICPU,Frame)

	//SSettings
	fprintf (S9xSTREAM, "\n//SSettings\n");
	OFFSET(APUEnabled, r1, SSettings,APUEnabled)
	OFFSET(Shutdown, r1, SSettings,Shutdown)
	OFFSET(SoundSkipMethod, r1, SSettings,SoundSkipMethod)
	OFFSET(H_Max, r1, SSettings,H_Max)
	OFFSET(HBlankStart, r1, SSettings,HBlankStart)
	OFFSET(CyclesPercentage, r1, SSettings,CyclesPercentage)
	OFFSET(DisableIRQ, r1, SSettings,DisableIRQ)
	OFFSET(Paused, r1, SSettings,Paused)
	OFFSET(PAL, r1, SSettings,PAL)
	OFFSET(SoundSync, r1, SSettings,SoundSync)
	OFFSET(SA1Enabled, r1, SSettings,SA1)
	OFFSET(SuperFXEnabled, r1, SSettings,SuperFX)

	//SAPURegisters
	fprintf (S9xSTREAM, "\n//SAPURegisters\n");
	OFFSET(ApuP, r1, SAPURegisters,P)
	OFFSET(ApuYA, r1, SAPURegisters,YA.W)
	OFFSET(ApuA, r1, SAPURegisters,YA.B.A)
	OFFSET(ApuY, r1, SAPURegisters,YA.B.Y)
	OFFSET(ApuX, r1, SAPURegisters,X)
	OFFSET(ApuS, r1, SAPURegisters,S)
	OFFSET(ApuPC, r1, SAPURegisters,PC)
	OFFSET(APUPCR, r1, SAPURegisters,PC)

	//SPPU
	fprintf (S9xSTREAM, "\n//SPPU\n");
	OFFSET(BGMode, r1, SPPU,BGMode)
	OFFSET(BG3Priority, r1, SPPU,BG3Priority)
	OFFSET(Brightness, r1, SPPU,Brightness)
	OFFSET(GHight, r1, SPPU,VMA.High)
	OFFSET(GInc, r1, SPPU,VMA.Increment)
	OFFSET(GAddress, r1, SPPU,VMA.Address)
	OFFSET(GMask1, r1, SPPU,VMA.Mask1)
	OFFSET(GFullGraphicCount, r1, SPPU,VMA.FullGraphicCount)
	OFFSET(GShift, r1, SPPU,VMA.Shift)
	OFFSET(CGFLIP, r1, SPPU,CGFLIP)
	OFFSET(CGDATA, r1, SPPU,CGDATA)
	OFFSET(FirstSprite, r1, SPPU,FirstSprite)
	OFFSET(LastSprite, r1, SPPU,LastSprite)
	OFFSET(OBJ, r1, SPPU,OBJ)
	OFFSET(OAMPriorityRotation, r1, SPPU,OAMPriorityRotation)
	OFFSET(OAMAddr, r1, SPPU,OAMAddr)
	OFFSET(OAMFlip, r1, SPPU,OAMFlip)
	OFFSET(OAMTileAddress, r1, SPPU,OAMTileAddress)
	OFFSET(IRQVBeamPos, r1, SPPU,IRQVBeamPos)
	OFFSET(IRQHBeamPos, r1, SPPU,IRQHBeamPos)
	OFFSET(VBeamPosLatched, r1, SPPU,VBeamPosLatched)
	OFFSET(HBeamPosLatched, r1, SPPU,HBeamPosLatched)
	OFFSET(HBeamFlip, r1, SPPU,HBeamFlip)
	OFFSET(VBeamFlip, r1, SPPU,VBeamFlip)
	OFFSET(HVBeamCounterLatched, r1, SPPU,HVBeamCounterLatched)
	OFFSET(MatrixA, r1, SPPU,MatrixA)
	OFFSET(MatrixB, r1, SPPU,MatrixB)
	OFFSET(MatrixC, r1, SPPU,MatrixC)
	OFFSET(MatrixD, r1, SPPU,MatrixD)
	OFFSET(CentreX, r1, SPPU,CentreX)
	OFFSET(CentreY, r1, SPPU,CentreY)
	OFFSET(Joypad1ButtonReadPos, r1, SPPU,Joypad1ButtonReadPos)
	OFFSET(Joypad2ButtonReadPos, r1, SPPU,Joypad2ButtonReadPos)
	OFFSET(CGADD, r1, SPPU,CGADD)
	OFFSET(FixedColourGreen, r1, SPPU,FixedColourGreen)
	OFFSET(FixedColourRed, r1, SPPU,FixedColourRed)
	OFFSET(FixedColourBlue, r1, SPPU,FixedColourBlue)
	OFFSET(SavedOAMAddr, r1, SPPU,SavedOAMAddr)
	OFFSET(ScreenHeight, r1, SPPU,ScreenHeight)
	OFFSET(WRAM, r1, SPPU,WRAM)
	OFFSET(BG_Forced, r1, SPPU,BG_Forced)
	OFFSET(ForcedBlanking, r1, SPPU,ForcedBlanking)
	OFFSET(OBJThroughMain, r1, SPPU,OBJThroughMain)
	OFFSET(OBJThroughSub, r1, SPPU,OBJThroughSub)
	OFFSET(OBJSizeSelect, r1, SPPU,OBJSizeSelect)
	OFFSET(OBJNameBase, r1, SPPU,OBJNameBase)
	OFFSET(OAMReadFlip, r1, SPPU,OAMReadFlip)
	OFFSET(OAMData, r1, SPPU,OAMData)
	OFFSET(VTimerEnabled, r1, SPPU,VTimerEnabled)
	OFFSET(HTimerEnabled, r1, SPPU,HTimerEnabled)
	OFFSET(HTimerPosition, r1, SPPU,HTimerPosition)
	OFFSET(Mosaic, r1, SPPU,Mosaic)
	OFFSET(BGMosaic, r1, SPPU,BGMosaic)
	OFFSET(Mode7HFlip, r1, SPPU,Mode7HFlip)
	OFFSET(Mode7VFlip, r1, SPPU,Mode7VFlip)
	OFFSET(Mode7Repeat, r1, SPPU,Mode7Repeat)
	OFFSET(Window1Left, r1, SPPU,Window1Left)
	OFFSET(Window1Right, r1, SPPU,Window1Right)
	OFFSET(Window2Left, r1, SPPU,Window2Left)
	OFFSET(Window2Right, r1, SPPU,Window2Right)
	OFFSET(ClipWindowOverlapLogic, r1, SPPU,ClipWindowOverlapLogic)
	OFFSET(ClipWindow1Enable, r1, SPPU,ClipWindow1Enable)
	OFFSET(ClipWindow2Enable, r1, SPPU,ClipWindow2Enable)
	OFFSET(ClipWindow1Inside, r1, SPPU,ClipWindow1Inside)
	OFFSET(ClipWindow2Inside, r1, SPPU,ClipWindow2Inside)
	OFFSET(RecomputeClipWindows, r1, SPPU,RecomputeClipWindows)
	OFFSET(CGFLIPRead, r1, SPPU,CGFLIPRead)
	OFFSET(OBJNameSelect, r1, SPPU,OBJNameSelect)
	OFFSET(Need16x8Mulitply, r1, SPPU,Need16x8Mulitply)
	OFFSET(Joypad3ButtonReadPos, r1, SPPU,Joypad3ButtonReadPos)
	OFFSET(MouseSpeed, r1, SPPU,MouseSpeed)
	OFFSET(RangeTimeOver, r1, SPPU,RangeTimeOver)

	//InternalPPU
	fprintf (S9xSTREAM, "\n//InternalPPU\n");
	OFFSET(ColorsChanged, r1, InternalPPU,ColorsChanged)
	OFFSET(HDMA, r1, InternalPPU,HDMA)
	OFFSET(HDMAStarted, r1, InternalPPU,HDMAStarted)
	OFFSET(MaxBrightness, r1, InternalPPU,MaxBrightness)
	OFFSET(LatchedBlanking, r1, InternalPPU,LatchedBlanking)
	OFFSET(OBJChanged, r1, InternalPPU,OBJChanged)
	OFFSET(RenderThisFrame, r1, InternalPPU,RenderThisFrame)
	OFFSET(SkippedFrames, r1, InternalPPU,SkippedFrames)
	OFFSET(FrameSkip, r1, InternalPPU,FrameSkip)
	OFFSET(TileCache, r1, InternalPPU,TileCache)
	OFFSET(TileCached, r1, InternalPPU,TileCached)
#ifdef CORRECT_VRAM_READS
	OFFSET(VRAMReadBuffer, r1, InternalPPU,VRAMReadBuffer)
#else
	OFFSET(FirstVRAMRead, r1, InternalPPU,FirstVRAMRead)
#endif
	OFFSET(Interlace, r1, InternalPPU,Interlace)
	OFFSET(DoubleWidthPixels, r1, InternalPPU,DoubleWidthPixels)
	OFFSET(RenderedScreenHeight, r1, InternalPPU,RenderedScreenHeight)
	OFFSET(RenderedScreenWidth, r1, InternalPPU,RenderedScreenWidth)
	OFFSET(Red, r1, InternalPPU,Red)
	OFFSET(Green, r1, InternalPPU,Green)
	OFFSET(Blue, r1, InternalPPU,Blue)
	OFFSET(XB, r1, InternalPPU,XB)
	OFFSET(ScreenColors, r1, InternalPPU,ScreenColors)
	OFFSET(PreviousLine, r1, InternalPPU,PreviousLine)
	OFFSET(CurrentLine, r1, InternalPPU,CurrentLine)
	OFFSET(Joypads, r1, InternalPPU,Joypads)
	OFFSET(SuperScope, r1, InternalPPU,SuperScope)
	OFFSET(Mouse, r1, InternalPPU,Mouse)
	OFFSET(PrevMouseX, r1, InternalPPU,PrevMouseX)
	OFFSET(PrevMouseY, r1, InternalPPU,PrevMouseY)
	OFFSET(Clip, r1, InternalPPU,Clip)

	//SSA1
	fprintf (S9xSTREAM, "\n//SSA1\n");
	OFFSET(SA1Opcodes, SA1REG, SSA1,S9xOpcodes)
	OFFSET(SA1_Carry, SA1REG, SSA1,_Carry)
	OFFSET(SA1_Zero, SA1REG, SSA1,_Zero)
	OFFSET(SA1_Negative, SA1REG, SSA1,_Negative)
	OFFSET(SA1_Overflow, SA1REG, SSA1,_Overflow)
	OFFSET(SA1CPUExecuting, SA1REG, SSA1,CPUExecuting)
	OFFSET(SA1ShiftedPB, SA1REG, SSA1,ShiftedPB)
	OFFSET(SA1ShiftedDB, SA1REG, SSA1,ShiftedDB)
	OFFSET(SA1Flags, SA1REG, SSA1,Flags)
	OFFSET(SA1Executing, SA1REG, SSA1,Executing)
	OFFSET(SA1NMIActive, SA1REG, SSA1,NMIActive)
	OFFSET(SA1IRQActive, SA1REG, SSA1,IRQActive)
	OFFSET(SA1WaitingForInterrupt, SA1REG, SSA1,WaitingForInterrupt)
	OFFSET(SA1PCS, SA1REG, SSA1,PC)
	OFFSET(SA1PCBase, SA1REG, SSA1,PCBase)
	OFFSET(SA1PCAtOpcodeStart, SA1REG, SSA1,PCAtOpcodeStart)
	OFFSET(SA1WaitAddress, SA1REG, SSA1,WaitAddress)
	OFFSET(SA1WaitCounter, SA1REG, SSA1,WaitCounter)
	OFFSET(SA1WaitByteAddress1, SA1REG, SSA1,WaitByteAddress1)
	OFFSET(SA1WaitByteAddress2, SA1REG, SSA1,WaitByteAddress2)
	OFFSET(SA1BWRAM, SA1REG, SSA1,BWRAM)
	OFFSET(SA1Map, SA1REG, SSA1,Map)
	OFFSET(SA1WriteMap, SA1REG, SSA1,WriteMap)
	OFFSET(SA1op1, SA1REG, SSA1,op1)
	OFFSET(SA1op2, SA1REG, SSA1,op2)
	OFFSET(SA1arithmetic_op, SA1REG, SSA1,arithmetic_op)
	OFFSET(SA1sum, SA1REG, SSA1,sum)
	OFFSET(SA1overflow, SA1REG, SSA1,overflow)
	OFFSET(VirtualBitmapFormat, SA1REG, SSA1,VirtualBitmapFormat)
	OFFSET(SA1_in_char_dma, SA1REG, SSA1,in_char_dma)
	OFFSET(SA1variable_bit_pos, SA1REG, SSA1,variable_bit_pos)

	//SSA1Registers
	fprintf (S9xSTREAM, "\n//SSA1Registers\n");
	OFFSET(SA1PB, r11, SSA1Registers, PB)
	OFFSET(SA1DB, r11, SSA1Registers, DB)
	OFFSET(SA1PP, r11, SSA1Registers, P.W)
	OFFSET(SA1PL, r11, SSA1Registers, P.W)
	fprintf (S9xSTREAM, "#define SA1PH SA1PL + 1\n");
	OFFSET(SA1AA, r11, SSA1Registers, A.W)
	OFFSET(SA1AL, r11, SSA1Registers, A.W)
	fprintf (S9xSTREAM, "#define SA1AH SA1AL + 1\n");
	OFFSET(SA1DD, r11, SSA1Registers, D.W)
	OFFSET(SA1DL, r11, SSA1Registers, D.W)
	fprintf (S9xSTREAM, "#define SA1DH SA1DL + 1\n");
	OFFSET(SA1SS, r11, SSA1Registers, S.W)
	OFFSET(SA1SL, r11, SSA1Registers, S.W)
	fprintf (S9xSTREAM, "#define SA1SH SA1SL + 1\n");
	OFFSET(SA1XX, r11, SSA1Registers, X.W)
	OFFSET(SA1XL, r11, SSA1Registers, X.W)
	fprintf (S9xSTREAM, "#define SA1XH SA1XL + 1\n");
	OFFSET(SA1YY, r11, SSA1Registers, Y.W)
	OFFSET(SA1YL, r11, SSA1Registers, Y.W)
	fprintf (S9xSTREAM, "#define SA1YH SA1YL + 1\n");
	OFFSET(SA1PCR, r11, SSA1Registers, PC)
	
	fprintf (S9xSTREAM, "\n//Other stuff\n");
	fprintf (S9xSTREAM, "#define S9xGetByte @(4,r14)\n");
	fprintf (S9xSTREAM, "#define S9xSetByte @(8,r14)\n");
	fprintf (S9xSTREAM, "#define S9xGetWord @(12,r14)\n");
	fprintf (S9xSTREAM, "#define S9xSetWord @(16,r14)\n");
	fprintf (S9xSTREAM, "#define S9xOpcode_IRQ @(20,r14)\n");
	fprintf (S9xSTREAM, "#define S9xOpcode_NMI @(24,r14)\n");
	fprintf (S9xSTREAM, "#define S9xSetPCBase @(28,r14)\n");
	fprintf (S9xSTREAM, "#define ICPU @(32,r14)\n");
	fprintf (S9xSTREAM, "#define IAPU @(36,r14)\n");
	fprintf (S9xSTREAM, "#define APU @(40,r14)\n");
	fprintf (S9xSTREAM, "#define SA1 @(44,r14)\n");
	fprintf (S9xSTREAM, "#define APURegisters @(48,r14)\n");
	fprintf (S9xSTREAM, "#define Settings @(52,r14)\n");
	fprintf (S9xSTREAM, "#define IPPU @(56,r14)\n");
	fprintf (S9xSTREAM, "#define PPU @(60,r14)\n");
	
	JUST_THE_NUMBER(SA1Map_offs, SSA1, Map);
	JUST_THE_NUMBER(SA1WriteMap_offs, SSA1, WriteMap);
	JUST_THE_NUMBER(NMITriggerPoint_offs, SCPUState, NMITriggerPoint);
	
	fprintf (S9xSTREAM, "\nEND OFFSETS\n");
}

//int main (int /*argc*/, char ** /*argv*/)
/*
void PrintOffsets()
{
	fprintf (S9xSTREAM, "START OFFSETS\n");
    OFFSET(Flags,Flags)
    OFFSET(BranchSkip,BranchSkip)
    OFFSET(NMIActive,NMIActive)
    OFFSET(IRQActive,IRQActive)
    OFFSET(WaitingForInterrupt,WaitingForInterrupt)
    OFFSET(InDMA,InDMA)
    OFFSET(WhichEvent,WhichEvent)
    OFFSET(PCS,PC)
    OFFSET(PCBase,PCBase)
    OFFSET(PCAtOpcodeStart,PCAtOpcodeStart)
    OFFSET(WaitAddress,WaitAddress)
    OFFSET(WaitCounter,WaitCounter)
    OFFSET(Cycles,Cycles)
    OFFSET(NextEvent,NextEvent)
    OFFSET(V_Counter,V_Counter)
    OFFSET(MemSpeed,MemSpeed)
    OFFSET(MemSpeedx2,MemSpeedx2)
    OFFSET(FastROMSpeed,FastROMSpeed)
    OFFSET(AutoSaveTimer,AutoSaveTimer)
    OFFSET(SRAMModified,SRAMModified)
    OFFSET(NMITriggerPoint,NMITriggerPoint)
    OFFSET(TriedInterleavedMode2,TriedInterleavedMode2)
    OFFSET(BRKTriggered,BRKTriggered)
    OFFSET(NMICycleCount,NMICycleCount)
    OFFSET(IRQCycleCount,IRQCycleCount)

    OFFSET2(PB,PB)
    OFFSET2(DB,DB)
    OFFSET2(PP,P.W)
    OFFSET2(PL,P.W)
    fprintf (S9xSTREAM, "#define PH PL + 1\n");
    OFFSET2(AA,A.W)
    OFFSET2(AL,A.W)
    fprintf (S9xSTREAM, "#define AH AL + 1\n");
    OFFSET2(DD,D.W)
    OFFSET2(DL,D.W)
    fprintf (S9xSTREAM, "#define DH DL + 1\n");
    OFFSET2(SS,S.W)
    OFFSET2(SL,S.W)
    fprintf (S9xSTREAM, "#define SH SL + 1\n");
    OFFSET2(XX,X.W)
    OFFSET2(XL,X.W)
    fprintf (S9xSTREAM, "#define XH XL + 1\n");
    OFFSET2(YY,Y.W)
    OFFSET2(YL,Y.W)
    fprintf (S9xSTREAM, "#define YH YL + 1\n");
    OFFSET2(PCR, PC)

    OFFSET3(RAM)
    OFFSET3(ROM)
    OFFSET3(VRAM)
    OFFSET3(SRAM)
    OFFSET3(BWRAM)
    OFFSET3(FillRAM)
    OFFSET3(C4RAM)
    OFFSET3(HiROM)
    OFFSET3(LoROM)
    OFFSET3(SRAMMask)
    OFFSET3(SRAMSize)
    OFFSET3(Map)
    OFFSET3(WriteMap)
    OFFSET3(MemorySpeed)
    OFFSET3(BlockIsRAM)
    OFFSET3(BlockIsROM)
    OFFSET3(ROMFilename)

    OFFSET5(APUPCS,PC)
    OFFSET5(APURAM,RAM)
    OFFSET5(APUExecuting,APUExecuting)
    OFFSET5(APUDirectPage,DirectPage)
    OFFSET5(APUBit,Bit)
    OFFSET5(APUAddress,Address)
    OFFSET5(APUWaitAddress1,WaitAddress1)
    OFFSET5(APUWaitAddress2,WaitAddress2)
    OFFSET5(APUWaitCounter,WaitCounter)
    OFFSET5(APUShadowRAM,ShadowRAM)
    OFFSET5(APUCachedSamples,CachedSamples)
    OFFSET5(APU_Carry,_Carry)
    OFFSET5(APU_Zero,_Zero)
    OFFSET5(APU_Overflow,_Overflow)
    OFFSET5(APUTimerErrorCounter,TimerErrorCounter)

    OFFSET4(APUCycles,Cycles)
    OFFSET4(APUShowROM,ShowROM)
    OFFSET4(APUFlags,Flags)
    OFFSET4(APUKeyedChannels,KeyedChannels)
    OFFSET4(APUOutPorts,OutPorts)
    OFFSET4(APUDSP,DSP)
    OFFSET4(APUExtraRAM,ExtraRAM)
    OFFSET4(APUTimer,Timer)
    OFFSET4(APUTimerTarget,TimerTarget)
    OFFSET4(APUTimerEnabled,TimerEnabled)
    OFFSET4(TimerValueWritten,TimerValueWritten)

    OFFSET6(CPUSpeed,Speed)
    OFFSET6(CPUOpcodes,S9xOpcodes)
    OFFSET6(_Carry,_Carry)
    OFFSET6(_Zero,_Zero)
    OFFSET6(_Negative,_Negative)
    OFFSET6(_Overflow,_Overflow)
    OFFSET6(ShiftedDB,ShiftedDB)
    OFFSET6(ShiftedPB,ShiftedPB)
    OFFSET6(CPUExecuting,CPUExecuting)
    OFFSET6(Scanline,Scanline)
    OFFSET6(Frame,Frame)

    OFFSET7(APUEnabled,APUEnabled)
    OFFSET7(Shutdown,Shutdown)
    OFFSET7(SoundSkipMethod,SoundSkipMethod)
    OFFSET7(H_Max,H_Max)
    OFFSET7(HBlankStart,HBlankStart)
    OFFSET7(CyclesPercentage,CyclesPercentage)
    OFFSET7(DisableIRQ,DisableIRQ)
    OFFSET7(Paused,Paused)
    OFFSET7(PAL,PAL)
    OFFSET7(SoundSync,SoundSync)
    OFFSET7(SA1Enabled,SA1)
    OFFSET7(SuperFXEnabled,SuperFX)

    OFFSET8(ApuP,P)
    OFFSET8(ApuYA,YA.W)
    OFFSET8(ApuA,YA.B.A)
    OFFSET8(ApuY,YA.B.Y)
    OFFSET8(ApuX,X)
    OFFSET8(ApuS,S)
    OFFSET8(ApuPC,PC)
    OFFSET8(APUPCR,PC)

    OFFSET9(BGMode,BGMode)
    OFFSET9(BG3Priority,BG3Priority)
    OFFSET9(Brightness,Brightness)
    OFFSET9(GHight,VMA.High)
    OFFSET9(GInc,VMA.Increment)
    OFFSET9(GAddress,VMA.Address)
    OFFSET9(GMask1,VMA.Mask1)
    OFFSET9(GFullGraphicCount,VMA.FullGraphicCount)
    OFFSET9(GShift,VMA.Shift)
    OFFSET9(CGFLIP,CGFLIP)
    OFFSET9(CGDATA,CGDATA)
    OFFSET9(FirstSprite,FirstSprite)
    OFFSET9(LastSprite,LastSprite)
    OFFSET9(OBJ,OBJ)
    OFFSET9(OAMPriorityRotation,OAMPriorityRotation)
    OFFSET9(OAMAddr,OAMAddr)
    OFFSET9(OAMFlip,OAMFlip)
    OFFSET9(OAMTileAddress,OAMTileAddress)
    OFFSET9(IRQVBeamPos,IRQVBeamPos)
    OFFSET9(IRQHBeamPos,IRQHBeamPos)
    OFFSET9(VBeamPosLatched,VBeamPosLatched)
    OFFSET9(HBeamPosLatched,HBeamPosLatched)
    OFFSET9(HBeamFlip,HBeamFlip)
    OFFSET9(VBeamFlip,VBeamFlip)
    OFFSET9(HVBeamCounterLatched,HVBeamCounterLatched)
    OFFSET9(MatrixA,MatrixA)
    OFFSET9(MatrixB,MatrixB)
    OFFSET9(MatrixC,MatrixC)
    OFFSET9(MatrixD,MatrixD)
    OFFSET9(CentreX,CentreX)
    OFFSET9(CentreY,CentreY)
    OFFSET9(Joypad1ButtonReadPos,Joypad1ButtonReadPos)
    OFFSET9(Joypad2ButtonReadPos,Joypad2ButtonReadPos)
    OFFSET9(CGADD,CGADD)
    OFFSET9(FixedColourGreen,FixedColourGreen)
    OFFSET9(FixedColourRed,FixedColourRed)
    OFFSET9(FixedColourBlue,FixedColourBlue)
    OFFSET9(SavedOAMAddr,SavedOAMAddr)
    OFFSET9(ScreenHeight,ScreenHeight)
    OFFSET9(WRAM,WRAM)
    OFFSET9(BG_Forced,BG_Forced)
    OFFSET9(ForcedBlanking,ForcedBlanking)
    OFFSET9(OBJThroughMain,OBJThroughMain)
    OFFSET9(OBJThroughSub,OBJThroughSub)
    OFFSET9(OBJSizeSelect,OBJSizeSelect)
    OFFSET9(OBJNameBase,OBJNameBase)
    OFFSET9(OAMReadFlip,OAMReadFlip)
    OFFSET9(OAMData,OAMData)
    OFFSET9(VTimerEnabled,VTimerEnabled)
    OFFSET9(HTimerEnabled,HTimerEnabled)
    OFFSET9(HTimerPosition,HTimerPosition)
    OFFSET9(Mosaic,Mosaic)
    OFFSET9(BGMosaic,BGMosaic)
    OFFSET9(Mode7HFlip,Mode7HFlip)
    OFFSET9(Mode7VFlip,Mode7VFlip)
    OFFSET9(Mode7Repeat,Mode7Repeat)
    OFFSET9(Window1Left,Window1Left)
    OFFSET9(Window1Right,Window1Right)
    OFFSET9(Window2Left,Window2Left)
    OFFSET9(Window2Right,Window2Right)
    OFFSET9(ClipWindowOverlapLogic,ClipWindowOverlapLogic)
    OFFSET9(ClipWindow1Enable,ClipWindow1Enable)
    OFFSET9(ClipWindow2Enable,ClipWindow2Enable)
    OFFSET9(ClipWindow1Inside,ClipWindow1Inside)
    OFFSET9(ClipWindow2Inside,ClipWindow2Inside)
    OFFSET9(RecomputeClipWindows,RecomputeClipWindows)
    OFFSET9(CGFLIPRead,CGFLIPRead)
    OFFSET9(OBJNameSelect,OBJNameSelect)
    OFFSET9(Need16x8Mulitply,Need16x8Mulitply)
    OFFSET9(Joypad3ButtonReadPos,Joypad3ButtonReadPos)
    OFFSET9(MouseSpeed,MouseSpeed)
    OFFSET9(RangeTimeOver,RangeTimeOver)

    OFFSET10(ColorsChanged,ColorsChanged)
    OFFSET10(HDMA,HDMA)
    OFFSET10(HDMAStarted,HDMAStarted)
    OFFSET10(MaxBrightness,MaxBrightness)
    OFFSET10(LatchedBlanking,LatchedBlanking)
    OFFSET10(OBJChanged,OBJChanged)
    OFFSET10(RenderThisFrame,RenderThisFrame)
    OFFSET10(SkippedFrames,SkippedFrames)
    OFFSET10(FrameSkip,FrameSkip)
    OFFSET10(TileCache,TileCache)
    OFFSET10(TileCached,TileCached)
#ifdef CORRECT_VRAM_READS
    OFFSET10(VRAMReadBuffer,VRAMReadBuffer)
#else
    OFFSET10(FirstVRAMRead,FirstVRAMRead)
#endif
    OFFSET10(Interlace,Interlace)
    OFFSET10(DoubleWidthPixels,DoubleWidthPixels)
    OFFSET10(RenderedScreenHeight,RenderedScreenHeight)
    OFFSET10(RenderedScreenWidth,RenderedScreenWidth)
    OFFSET10(Red,Red)
    OFFSET10(Green,Green)
    OFFSET10(Blue,Blue)
    OFFSET10(XB,XB)
    OFFSET10(ScreenColors,ScreenColors)
    OFFSET10(PreviousLine,PreviousLine)
    OFFSET10(CurrentLine,CurrentLine)
    OFFSET10(Joypads,Joypads)
    OFFSET10(SuperScope,SuperScope)
    OFFSET10(Mouse,Mouse)
    OFFSET10(PrevMouseX,PrevMouseX)
    OFFSET10(PrevMouseY,PrevMouseY)
    OFFSET10(Clip,Clip)

    OFFSET11(SA1Opcodes,S9xOpcodes)
    OFFSET11(SA1_Carry,_Carry)
    OFFSET11(SA1_Zero,_Zero)
    OFFSET11(SA1_Negative,_Negative)
    OFFSET11(SA1_Overflow,_Overflow)
    OFFSET11(SA1CPUExecuting,CPUExecuting)
    OFFSET11(SA1ShiftedPB,ShiftedPB)
    OFFSET11(SA1ShiftedDB,ShiftedDB)
    OFFSET11(SA1Flags,Flags)
    OFFSET11(SA1Executing,Executing)
    OFFSET11(SA1NMIActive,NMIActive)
    OFFSET11(SA1IRQActive,IRQActive)
    OFFSET11(SA1WaitingForInterrupt,WaitingForInterrupt)
    OFFSET11(SA1PCS,PC)
    OFFSET11(SA1PCBase,PCBase)
    OFFSET11(SA1PCAtOpcodeStart,PCAtOpcodeStart)
    OFFSET11(SA1WaitAddress,WaitAddress)
    OFFSET11(SA1WaitCounter,WaitCounter)
    OFFSET11(SA1WaitByteAddress1,WaitByteAddress1)
    OFFSET11(SA1WaitByteAddress2,WaitByteAddress2)
    OFFSET11(SA1BWRAM,BWRAM)
    OFFSET11(SA1Map,Map)
    OFFSET11(SA1WriteMap,WriteMap)
    OFFSET11(SA1op1,op1)
    OFFSET11(SA1op2,op2)
    OFFSET11(SA1arithmetic_op,arithmetic_op)
    OFFSET11(SA1sum,sum)
    OFFSET11(SA1overflow,overflow)
    OFFSET11(VirtualBitmapFormat,VirtualBitmapFormat)
    OFFSET11(SA1_in_char_dma,in_char_dma)
    OFFSET11(SA1variable_bit_pos,variable_bit_pos)

    OFFSET12(SA1PB,PB)
    OFFSET12(SA1DB,DB)
    OFFSET12(SA1PP,P.W)
    OFFSET12(SA1PL,P.W)
    fprintf (S9xSTREAM, "#define SA1PH SA1PL + 1\n");
    OFFSET12(SA1AA,A.W)
    OFFSET12(SA1AL,A.W)
    fprintf (S9xSTREAM, "#define SA1AH SA1AL + 1\n");
    OFFSET12(SA1DD,D.W)
    OFFSET12(SA1DL,D.W)
    fprintf (S9xSTREAM, "#define SA1DH SA1DL + 1\n");
    OFFSET12(SA1SS,S.W)
    OFFSET12(SA1SL,S.W)
    fprintf (S9xSTREAM, "#define SA1SH SA1SL + 1\n");
    OFFSET12(SA1XX,X.W)
    OFFSET12(SA1XL,X.W)
    fprintf (S9xSTREAM, "#define SA1XH SA1XL + 1\n");
    OFFSET12(SA1YY,Y.W)
    OFFSET12(SA1YL,Y.W)
    fprintf (S9xSTREAM, "#define SA1YH SA1YL + 1\n");
    OFFSET12(SA1PCR, PC)
	fprintf (S9xSTREAM, "END OFFSETS\n");
}

*/