! global struct member symbols

.macro LOCSYM_Flags LocalName
\LocalName:
	.long	_CPU + 0
.endm

.macro LOCSYM_BranchSkip LocalName
\LocalName:
	.long	CPU + 4
.endm

.macro LOCSYM_NMIActive LocalName
\LocalName:
	.long	_CPU + 5
.endm

.macro LOCSYM_IRQActive LocalName
\LocalName:
	.long	_CPU + 6
.endm

.macro LOCSYM_WaitingForInterrupt LocalName
\LocalName:
	.long	_CPU + 7
.endm

.macro LOCSYM_InDMA LocalName
\LocalName:
	.long	_CPU + 8
.endm

.macro LOCSYM_WhichEvent LocalName
\LocalName:
	.long	_CPU + 9
.endm

.macro LOCSYM_PCS LocalName
\LocalName:
	.long	_CPU + 12
.endm

.macro LOCSYM_PCBase LocalName
\LocalName:
	.long	_CPU + 16
.endm

.macro LOCSYM_PCAtOpcodeStart LocalName
\LocalName:
	.long	_CPU + 20
.endm

.macro LOCSYM_WaitAddress LocalName
\LocalName:
	.long	_CPU + 24
.endm

.macro LOCSYM_WaitCounter LocalName
\LocalName:
	.long	_CPU + 28
.endm

.macro LOCSYM_Cycles LocalName
\LocalName:
	.long	_CPU + 32
.endm

.macro LOCSYM_NextEvent LocalName
\LocalName:
	.long	_CPU + 36
.endm

.macro LOCSYM_V_Counter LocalName
\LocalName:
	.long	_CPU + 40
.endm

.macro LOCSYM_MemSpeed LocalName
\LocalName:
	.long	_CPU + 44
.endm

.macro LOCSYM_MemSpeedx2 LocalName
\LocalName:
	.long	_CPU + 48
.endm

.macro LOCSYM_FastROMSpeed LocalName
\LocalName:
	.long	_CPU + 52
.endm

.macro LOCSYM_AutoSaveTimer LocalName
\LocalName:
	.long	_CPU + 56
.endm

.macro LOCSYM_SRAMModified LocalName
\LocalName:
	.long	_CPU + 60
.endm

.macro LOCSYM_NMITriggerPoint LocalName
\LocalName:
	.long	_CPU + 64
.endm

.macro LOCSYM_TriedInterleavedMode2 LocalName
\LocalName:
	.long	_CPU + 69
.endm

.macro LOCSYM_BRKTriggered LocalName
\LocalName:
	.long	_CPU + 68
.endm

.macro LOCSYM_NMICycleCount LocalName
\LocalName:
	.long	_CPU + 72
.endm

.macro LOCSYM_IRQCycleCount LocalName
\LocalName:
	.long	_CPU + 76
.endm

.macro LOCSYM_PB LocalName
\LocalName:
	.long	_Registers + 0
.endm

.macro LOCSYM_DB LocalName
\LocalName:
	.long	_Registers + 1
.endm

.macro LOCSYM_PP LocalName
\LocalName:
	.long	_Registers + 2
.endm

.macro LOCSYM_PL LocalName
\LocalName:
	.long	_Registers + 2
.endm

.macro LOCSYM_PH LocalName
\LocalName:
	.long	PL + 1
.endm

.macro LOCSYM_AA LocalName
\LocalName:
	.long	_Registers + 4
.endm

.macro LOCSYM_AL LocalName
\LocalName:
	.long	_Registers + 4
.endm

.macro LOCSYM_AH LocalName
\LocalName:
	.long	AL + 1
.endm

.macro LOCSYM_DD LocalName
\LocalName:
	.long	_Registers + 6
.endm

.macro LOCSYM_DL LocalName
\LocalName:
	.long	_Registers + 6
.endm

.macro LOCSYM_DH LocalName
\LocalName:
	.long	DL + 1
.endm

.macro LOCSYM_SS LocalName
\LocalName:
	.long	_Registers + 8
.endm

.macro LOCSYM_SL LocalName
\LocalName:
	.long	_Registers + 8
.endm

.macro LOCSYM_SH LocalName
\LocalName:
	.long	SL + 1
.endm

.macro LOCSYM_XX LocalName
\LocalName:
	.long	_Registers + 10
.endm

.macro LOCSYM_XL LocalName
\LocalName:
	.long	_Registers + 10
.endm

.macro LOCSYM_XH LocalName
\LocalName:
	.long	XL + 1
.endm

.macro LOCSYM_YY LocalName
\LocalName:
	.long	_Registers + 12
.endm

.macro LOCSYM_YL LocalName
\LocalName:
	.long	_Registers + 12
.endm

.macro LOCSYM_YH LocalName
\LocalName:
	.long	YL + 1
.endm

.macro LOCSYM_PCR LocalName
\LocalName:
	.long	_Registers + 14
.endm

.macro LOCSYM_RAM LocalName
\LocalName:
	.long	_Memory + 0
.endm

.macro LOCSYM_ROM LocalName
\LocalName:
	.long	_Memory + 4
.endm

.macro LOCSYM_VRAM LocalName
\LocalName:
	.long	_Memory + 8
.endm

.macro LOCSYM_SRAM LocalName
\LocalName:
	.long	_Memory + 12
.endm

.macro LOCSYM_BWRAM LocalName
\LocalName:
	.long	_Memory + 16
.endm

.macro LOCSYM_FillRAM LocalName
\LocalName:
	.long	_Memory + 20
.endm

.macro LOCSYM_C4RAM LocalName
\LocalName:
	.long	_Memory + 24
.endm

.macro LOCSYM_HiROM LocalName
\LocalName:
	.long	_Memory + 28
.endm

.macro LOCSYM_LoROM LocalName
\LocalName:
	.long	_Memory + 29
.endm

.macro LOCSYM_SRAMMask LocalName
\LocalName:
	.long	_Memory + 32
.endm

.macro LOCSYM_SRAMSize LocalName
\LocalName:
	.long	_Memory + 36
.endm

.macro LOCSYM_Map LocalName
\LocalName:
	.long	_Memory + 40
.endm

.macro LOCSYM_WriteMap LocalName
\LocalName:
	.long	_Memory + 16424
.endm

.macro LOCSYM_MemorySpeed LocalName
\LocalName:
	.long	_Memory + 32808
.endm

.macro LOCSYM_BlockIsRAM LocalName
\LocalName:
	.long	_Memory + 36904
.endm

.macro LOCSYM_BlockIsROM LocalName
\LocalName:
	.long	_Memory + 41000
.endm

.macro LOCSYM_ROMFilename LocalName
\LocalName:
	.long	_Memory + 53368
.endm

.macro LOCSYM_APUPCS LocalName
\LocalName:
	.long	_IAPU + 0
.endm

.macro LOCSYM_APURAM LocalName
\LocalName:
	.long	_IAPU + 4
.endm

.macro LOCSYM_APUExecuting LocalName
\LocalName:
	.long	_IAPU + 12
.endm

.macro LOCSYM_APUDirectPage LocalName
\LocalName:
	.long	_IAPU + 8
.endm

.macro LOCSYM_APUBit LocalName
\LocalName:
	.long	_IAPU + 13
.endm

.macro LOCSYM_APUAddress LocalName
\LocalName:
	.long	_IAPU + 16
.endm

.macro LOCSYM_APUWaitAddress1 LocalName
\LocalName:
	.long	_IAPU + 20
.endm

.macro LOCSYM_APUWaitAddress2 LocalName
\LocalName:
	.long	_IAPU + 24
.endm

.macro LOCSYM_APUWaitCounter LocalName
\LocalName:
	.long	_IAPU + 28
.endm

.macro LOCSYM_APUShadowRAM LocalName
\LocalName:
	.long	_IAPU + 32
.endm

.macro LOCSYM_APUCachedSamples LocalName
\LocalName:
	.long	_IAPU + 36
.endm

.macro LOCSYM_APU_Carry LocalName
\LocalName:
	.long	_IAPU + 40
.endm

.macro LOCSYM_APU_Zero LocalName
\LocalName:
	.long	_IAPU + 41
.endm

.macro LOCSYM_APU_Overflow LocalName
\LocalName:
	.long	_IAPU + 42
.endm

.macro LOCSYM_APUTimerErrorCounter LocalName
\LocalName:
	.long	_IAPU + 44
.endm

.macro LOCSYM_APUCycles LocalName
\LocalName:
	.long	_APU + 0
.endm

.macro LOCSYM_APUShowROM LocalName
\LocalName:
	.long	_APU + 4
.endm

.macro LOCSYM_APUFlags LocalName
\LocalName:
	.long	_APU + 5
.endm

.macro LOCSYM_APUKeyedChannels LocalName
\LocalName:
	.long	_APU + 6
.endm

.macro LOCSYM_APUOutPorts LocalName
\LocalName:
	.long	_APU + 7
.endm

.macro LOCSYM_APUDSP LocalName
\LocalName:
	.long	_APU + 11
.endm

.macro LOCSYM_APUExtraRAM LocalName
\LocalName:
	.long	_APU + 139
.endm

.macro LOCSYM_APUTimer LocalName
\LocalName:
	.long	_APU + 204
.endm

.macro LOCSYM_APUTimerTarget LocalName
\LocalName:
	.long	_APU + 210
.endm

.macro LOCSYM_APUTimerEnabled LocalName
\LocalName:
	.long	_APU + 216
.endm

.macro LOCSYM_TimerValueWritten LocalName
\LocalName:
	.long	_APU + 219
.endm

.macro LOCSYM_CPUSpeed LocalName
\LocalName:
	.long	_ICPU + 0
.endm

.macro LOCSYM_CPUOpcodes LocalName
\LocalName:
	.long	_ICPU + 4
.endm

.macro LOCSYM__Carry LocalName
\LocalName:
	.long	_ICPU + 8
.endm

.macro LOCSYM__Zero LocalName
\LocalName:
	.long	_ICPU + 9
.endm

.macro LOCSYM__Negative LocalName
\LocalName:
	.long	_ICPU + 10
.endm

.macro LOCSYM__Overflow LocalName
\LocalName:
	.long	_ICPU + 11
.endm

.macro LOCSYM_ShiftedDB LocalName
\LocalName:
	.long	_ICPU + 20
.endm

.macro LOCSYM_ShiftedPB LocalName
\LocalName:
	.long	_ICPU + 16
.endm

.macro LOCSYM_CPUExecuting LocalName
\LocalName:
	.long	_ICPU + 12
.endm

.macro LOCSYM_Scanline LocalName
\LocalName:
	.long	_ICPU + 28
.endm

.macro LOCSYM_Frame LocalName
\LocalName:
	.long	_ICPU + 24
.endm

.macro LOCSYM_APUEnabled LocalName
\LocalName:
	.long	_Settings + 0
.endm

.macro LOCSYM_Shutdown LocalName
\LocalName:
	.long	_Settings + 1
.endm

.macro LOCSYM_SoundSkipMethod LocalName
\LocalName:
	.long	_Settings + 2
.endm

.macro LOCSYM_H_Max LocalName
\LocalName:
	.long	_Settings + 4
.endm

.macro LOCSYM_HBlankStart LocalName
\LocalName:
	.long	_Settings + 8
.endm

.macro LOCSYM_CyclesPercentage LocalName
\LocalName:
	.long	_Settings + 12
.endm

.macro LOCSYM_DisableIRQ LocalName
\LocalName:
	.long	_Settings + 16
.endm

.macro LOCSYM_Paused LocalName
\LocalName:
	.long	_Settings + 17
.endm

.macro LOCSYM_PAL LocalName
\LocalName:
	.long	_Settings + 29
.endm

.macro LOCSYM_SoundSync LocalName
\LocalName:
	.long	_Settings + 108
.endm

.macro LOCSYM_SA1Enabled LocalName
\LocalName:
	.long	_Settings + 82
.endm

.macro LOCSYM_SuperFXEnabled LocalName
\LocalName:
	.long	_Settings + 80
.endm

.macro LOCSYM_ApuP LocalName
\LocalName:
	.long	_APURegisters + 0
.endm

.macro LOCSYM_ApuYA LocalName
\LocalName:
	.long	_APURegisters + 2
.endm

.macro LOCSYM_ApuA LocalName
\LocalName:
	.long	_APURegisters + 2
.endm

.macro LOCSYM_ApuY LocalName
\LocalName:
	.long	_APURegisters + 3
.endm

.macro LOCSYM_ApuX LocalName
\LocalName:
	.long	_APURegisters + 4
.endm

.macro LOCSYM_ApuS LocalName
\LocalName:
	.long	_APURegisters + 5
.endm

.macro LOCSYM_ApuPC LocalName
\LocalName:
	.long	_APURegisters + 6
.endm

.macro LOCSYM_APUPCR LocalName
\LocalName:
	.long	_APURegisters + 6
.endm

.macro LOCSYM_BGMode LocalName
\LocalName:
	.long	_PPU + 0
.endm

.macro LOCSYM_BG3Priority LocalName
\LocalName:
	.long	_PPU + 1
.endm

.macro LOCSYM_Brightness LocalName
\LocalName:
	.long	_PPU + 2
.endm

.macro LOCSYM_GHight LocalName
\LocalName:
	.long	_PPU + 4
.endm

.macro LOCSYM_GInc LocalName
\LocalName:
	.long	_PPU + 5
.endm

.macro LOCSYM_GAddress LocalName
\LocalName:
	.long	_PPU + 6
.endm

.macro LOCSYM_GMask1 LocalName
\LocalName:
	.long	_PPU + 8
.endm

.macro LOCSYM_GFullGraphicCount LocalName
\LocalName:
	.long	_PPU + 10
.endm

.macro LOCSYM_GShift LocalName
\LocalName:
	.long	_PPU + 12
.endm

.macro LOCSYM_CGFLIP LocalName
\LocalName:
	.long	_PPU + 62
.endm

.macro LOCSYM_CGDATA LocalName
\LocalName:
	.long	_PPU + 64
.endm

.macro LOCSYM_FirstSprite LocalName
\LocalName:
	.long	_PPU + 576
.endm

.macro LOCSYM_LastSprite LocalName
\LocalName:
	.long	_PPU + 577
.endm

.macro LOCSYM_OBJ LocalName
\LocalName:
	.long	_PPU + 578
.endm

.macro LOCSYM_OAMPriorityRotation LocalName
\LocalName:
	.long	_PPU + 2114
.endm

.macro LOCSYM_OAMAddr LocalName
\LocalName:
	.long	_PPU + 2116
.endm

.macro LOCSYM_OAMFlip LocalName
\LocalName:
	.long	_PPU + 2119
.endm

.macro LOCSYM_OAMTileAddress LocalName
\LocalName:
	.long	_PPU + 2120
.endm

.macro LOCSYM_IRQVBeamPos LocalName
\LocalName:
	.long	_PPU + 2122
.endm

.macro LOCSYM_IRQHBeamPos LocalName
\LocalName:
	.long	_PPU + 2124
.endm

.macro LOCSYM_VBeamPosLatched LocalName
\LocalName:
	.long	_PPU + 2126
.endm

.macro LOCSYM_HBeamPosLatched LocalName
\LocalName:
	.long	_PPU + 2128
.endm

.macro LOCSYM_HBeamFlip LocalName
\LocalName:
	.long	_PPU + 2130
.endm

.macro LOCSYM_VBeamFlip LocalName
\LocalName:
	.long	_PPU + 2131
.endm

.macro LOCSYM_HVBeamCounterLatched LocalName
\LocalName:
	.long	_PPU + 2132
.endm

.macro LOCSYM_MatrixA LocalName
\LocalName:
	.long	_PPU + 2134
.endm

.macro LOCSYM_MatrixB LocalName
\LocalName:
	.long	_PPU + 2136
.endm

.macro LOCSYM_MatrixC LocalName
\LocalName:
	.long	_PPU + 2138
.endm

.macro LOCSYM_MatrixD LocalName
\LocalName:
	.long	_PPU + 2140
.endm

.macro LOCSYM_CentreX LocalName
\LocalName:
	.long	_PPU + 2142
.endm

.macro LOCSYM_CentreY LocalName
\LocalName:
	.long	_PPU + 2144
.endm

.macro LOCSYM_Joypad1ButtonReadPos LocalName
\LocalName:
	.long	_PPU + 2146
.endm

.macro LOCSYM_Joypad2ButtonReadPos LocalName
\LocalName:
	.long	_PPU + 2147
.endm

.macro LOCSYM_CGADD LocalName
\LocalName:
	.long	_PPU + 2148
.endm

.macro LOCSYM_FixedColourGreen LocalName
\LocalName:
	.long	_PPU + 2150
.endm

.macro LOCSYM_FixedColourRed LocalName
\LocalName:
	.long	_PPU + 2149
.endm

.macro LOCSYM_FixedColourBlue LocalName
\LocalName:
	.long	_PPU + 2151
.endm

.macro LOCSYM_SavedOAMAddr LocalName
\LocalName:
	.long	_PPU + 2152
.endm

.macro LOCSYM_ScreenHeight LocalName
\LocalName:
	.long	_PPU + 2154
.endm

.macro LOCSYM_WRAM LocalName
\LocalName:
	.long	_PPU + 2156
.endm

.macro LOCSYM_BG_Forced LocalName
\LocalName:
	.long	_PPU + 2160
.endm

.macro LOCSYM_ForcedBlanking LocalName
\LocalName:
	.long	_PPU + 2161
.endm

.macro LOCSYM_OBJThroughMain LocalName
\LocalName:
	.long	_PPU + 2162
.endm

.macro LOCSYM_OBJThroughSub LocalName
\LocalName:
	.long	_PPU + 2163
.endm

.macro LOCSYM_OBJSizeSelect LocalName
\LocalName:
	.long	_PPU + 2164
.endm

.macro LOCSYM_OBJNameBase LocalName
\LocalName:
	.long	_PPU + 2166
.endm

.macro LOCSYM_OAMReadFlip LocalName
\LocalName:
	.long	_PPU + 2169
.endm

.macro LOCSYM_OAMData LocalName
\LocalName:
	.long	_PPU + 2170
.endm

.macro LOCSYM_VTimerEnabled LocalName
\LocalName:
	.long	_PPU + 2714
.endm

.macro LOCSYM_HTimerEnabled LocalName
\LocalName:
	.long	_PPU + 2715
.endm

.macro LOCSYM_HTimerPosition LocalName
\LocalName:
	.long	_PPU + 2716
.endm

.macro LOCSYM_Mosaic LocalName
\LocalName:
	.long	_PPU + 2718
.endm

.macro LOCSYM_BGMosaic LocalName
\LocalName:
	.long	_PPU + 2719
.endm

.macro LOCSYM_Mode7HFlip LocalName
\LocalName:
	.long	_PPU + 2723
.endm

.macro LOCSYM_Mode7VFlip LocalName
\LocalName:
	.long	_PPU + 2724
.endm

.macro LOCSYM_Mode7Repeat LocalName
\LocalName:
	.long	_PPU + 2725
.endm

.macro LOCSYM_Window1Left LocalName
\LocalName:
	.long	_PPU + 2726
.endm

.macro LOCSYM_Window1Right LocalName
\LocalName:
	.long	_PPU + 2727
.endm

.macro LOCSYM_Window2Left LocalName
\LocalName:
	.long	_PPU + 2728
.endm

.macro LOCSYM_Window2Right LocalName
\LocalName:
	.long	_PPU + 2729
.endm

.macro LOCSYM_ClipWindowOverlapLogic LocalName
\LocalName:
	.long	_PPU + 2736
.endm

.macro LOCSYM_ClipWindow1Enable LocalName
\LocalName:
	.long	_PPU + 2742
.endm

.macro LOCSYM_ClipWindow2Enable LocalName
\LocalName:
	.long	_PPU + 2748
.endm

.macro LOCSYM_ClipWindow1Inside LocalName
\LocalName:
	.long	_PPU + 2754
.endm

.macro LOCSYM_ClipWindow2Inside LocalName
\LocalName:
	.long	_PPU + 2760
.endm

.macro LOCSYM_RecomputeClipWindows LocalName
\LocalName:
	.long	_PPU + 2766
.endm

.macro LOCSYM_CGFLIPRead LocalName
\LocalName:
	.long	_PPU + 2767
.endm

.macro LOCSYM_OBJNameSelect LocalName
\LocalName:
	.long	_PPU + 2768
.endm

.macro LOCSYM_Need16x8Mulitply LocalName
\LocalName:
	.long	_PPU + 2770
.endm

.macro LOCSYM_Joypad3ButtonReadPos LocalName
\LocalName:
	.long	_PPU + 2771
.endm

.macro LOCSYM_MouseSpeed LocalName
\LocalName:
	.long	_PPU + 2772
.endm

.macro LOCSYM_RangeTimeOver LocalName
\LocalName:
	.long	_PPU + 2118
.endm

.macro LOCSYM_ColorsChanged LocalName
\LocalName:
	.long	_IPPU + 0
.endm

.macro LOCSYM_HDMA LocalName
\LocalName:
	.long	_IPPU + 1
.endm

.macro LOCSYM_HDMAStarted LocalName
\LocalName:
	.long	_IPPU + 2
.endm

.macro LOCSYM_MaxBrightness LocalName
\LocalName:
	.long	_IPPU + 3
.endm

.macro LOCSYM_LatchedBlanking LocalName
\LocalName:
	.long	_IPPU + 4
.endm

.macro LOCSYM_OBJChanged LocalName
\LocalName:
	.long	_IPPU + 5
.endm

.macro LOCSYM_RenderThisFrame LocalName
\LocalName:
	.long	_IPPU + 6
.endm

.macro LOCSYM_SkippedFrames LocalName
\LocalName:
	.long	_IPPU + 20
.endm

.macro LOCSYM_FrameSkip LocalName
\LocalName:
	.long	_IPPU + 24
.endm

.macro LOCSYM_TileCache LocalName
\LocalName:
	.long	_IPPU + 28
.endm

.macro LOCSYM_TileCached LocalName
\LocalName:
	.long	_IPPU + 40
.endm

.macro LOCSYM_FirstVRAMRead LocalName
\LocalName:
	.long	_IPPU + 52
.endm

.macro LOCSYM_Interlace LocalName
\LocalName:
	.long	_IPPU + 54
.endm

.macro LOCSYM_DoubleWidthPixels LocalName
\LocalName:
	.long	_IPPU + 56
.endm

.macro LOCSYM_RenderedScreenHeight LocalName
\LocalName:
	.long	_IPPU + 60
.endm

.macro LOCSYM_RenderedScreenWidth LocalName
\LocalName:
	.long	_IPPU + 64
.endm

.macro LOCSYM_Red LocalName
\LocalName:
	.long	_IPPU + 68
.endm

.macro LOCSYM_Green LocalName
\LocalName:
	.long	_IPPU + 1092
.endm

.macro LOCSYM_Blue LocalName
\LocalName:
	.long	_IPPU + 2116
.endm

.macro LOCSYM_XB LocalName
\LocalName:
	.long	_IPPU + 3140
.endm

.macro LOCSYM_ScreenColors LocalName
\LocalName:
	.long	_IPPU + 3144
.endm

.macro LOCSYM_PreviousLine LocalName
\LocalName:
	.long	_IPPU + 3656
.endm

.macro LOCSYM_CurrentLine LocalName
\LocalName:
	.long	_IPPU + 3660
.endm

.macro LOCSYM_Joypads LocalName
\LocalName:
	.long	_IPPU + 3668
.endm

.macro LOCSYM_SuperScope LocalName
\LocalName:
	.long	_IPPU + 3688
.endm

.macro LOCSYM_Mouse LocalName
\LocalName:
	.long	_IPPU + 3692
.endm

.macro LOCSYM_PrevMouseX LocalName
\LocalName:
	.long	_IPPU + 3700
.endm

.macro LOCSYM_PrevMouseY LocalName
\LocalName:
	.long	_IPPU + 3708
.endm

.macro LOCSYM_Clip LocalName
\LocalName:
	.long	_IPPU + 3716
.endm

.macro LOCSYM_SA1Opcodes LocalName
\LocalName:
	.long	_SA1 + 0
.endm

.macro LOCSYM_SA1_Carry LocalName
\LocalName:
	.long	_SA1 + 4
.endm

.macro LOCSYM_SA1_Zero LocalName
\LocalName:
	.long	_SA1 + 5
.endm

.macro LOCSYM_SA1_Negative LocalName
\LocalName:
	.long	_SA1 + 6
.endm

.macro LOCSYM_SA1_Overflow LocalName
\LocalName:
	.long	_SA1 + 7
.endm

.macro LOCSYM_SA1CPUExecuting LocalName
\LocalName:
	.long	_SA1 + 8
.endm

.macro LOCSYM_SA1ShiftedPB LocalName
\LocalName:
	.long	_SA1 + 12
.endm

.macro LOCSYM_SA1ShiftedDB LocalName
\LocalName:
	.long	_SA1 + 16
.endm

.macro LOCSYM_SA1Flags LocalName
\LocalName:
	.long	_SA1 + 20
.endm

.macro LOCSYM_SA1Executing LocalName
\LocalName:
	.long	_SA1 + 24
.endm

.macro LOCSYM_SA1NMIActive LocalName
\LocalName:
	.long	_SA1 + 25
.endm

.macro LOCSYM_SA1IRQActive LocalName
\LocalName:
	.long	_SA1 + 26
.endm

.macro LOCSYM_SA1WaitingForInterrupt LocalName
\LocalName:
	.long	_SA1 + 27
.endm

.macro LOCSYM_SA1PCS LocalName
\LocalName:
	.long	_SA1 + 32
.endm

.macro LOCSYM_SA1PCBase LocalName
\LocalName:
	.long	_SA1 + 36
.endm

.macro LOCSYM_SA1PCAtOpcodeStart LocalName
\LocalName:
	.long	_SA1 + 44
.endm

.macro LOCSYM_SA1WaitAddress LocalName
\LocalName:
	.long	_SA1 + 48
.endm

.macro LOCSYM_SA1WaitCounter LocalName
\LocalName:
	.long	_SA1 + 52
.endm

.macro LOCSYM_SA1WaitByteAddress1 LocalName
\LocalName:
	.long	_SA1 + 56
.endm

.macro LOCSYM_SA1WaitByteAddress2 LocalName
\LocalName:
	.long	_SA1 + 60
.endm

.macro LOCSYM_SA1BWRAM LocalName
\LocalName:
	.long	_SA1 + 40
.endm

.macro LOCSYM_SA1Map LocalName
\LocalName:
	.long	_SA1 + 64
.endm

.macro LOCSYM_SA1WriteMap LocalName
\LocalName:
	.long	_SA1 + 16448
.endm

.macro LOCSYM_SA1op1 LocalName
\LocalName:
	.long	_SA1 + 32832
.endm

.macro LOCSYM_SA1op2 LocalName
\LocalName:
	.long	_SA1 + 32834
.endm

.macro LOCSYM_SA1arithmetic_op LocalName
\LocalName:
	.long	_SA1 + 32836
.endm

.macro LOCSYM_SA1sum LocalName
\LocalName:
	.long	_SA1 + 32840
.endm

.macro LOCSYM_SA1overflow LocalName
\LocalName:
	.long	_SA1 + 32848
.endm

.macro LOCSYM_VirtualBitmapFormat LocalName
\LocalName:
	.long	_SA1 + 32849
.endm

.macro LOCSYM_SA1_in_char_dma LocalName
\LocalName:
	.long	_SA1 + 32850
.endm

.macro LOCSYM_SA1variable_bit_pos LocalName
\LocalName:
	.long	_SA1 + 32851
.endm

.macro LOCSYM_SA1PB LocalName
\LocalName:
	.long	_SA1Registers + 0
.endm

.macro LOCSYM_SA1DB LocalName
\LocalName:
	.long	_SA1Registers + 1
.endm

.macro LOCSYM_SA1PP LocalName
\LocalName:
	.long	_SA1Registers + 2
.endm

.macro LOCSYM_SA1PL LocalName
\LocalName:
	.long	_SA1Registers + 2
.endm

.macro LOCSYM_SA1PH LocalName
\LocalName:
	.long	SA1PL + 1
.endm

.macro LOCSYM_SA1AA LocalName
\LocalName:
	.long	_SA1Registers + 4
.endm

.macro LOCSYM_SA1AL LocalName
\LocalName:
	.long	_SA1Registers + 4
.endm

.macro LOCSYM_SA1AH LocalName
\LocalName:
	.long	SA1AL + 1
.endm

.macro LOCSYM_SA1DD LocalName
\LocalName:
	.long	_SA1Registers + 6
.endm

.macro LOCSYM_SA1DL LocalName
\LocalName:
	.long	_SA1Registers + 6
.endm

.macro LOCSYM_SA1DH LocalName
\LocalName:
	.long	SA1DL + 1
.endm

.macro LOCSYM_SA1SS LocalName
\LocalName:
	.long	_SA1Registers + 8
.endm

.macro LOCSYM_SA1SL LocalName
\LocalName:
	.long	_SA1Registers + 8
.endm

.macro LOCSYM_SA1SH LocalName
\LocalName:
	.long	SA1SL + 1
.endm

.macro LOCSYM_SA1XX LocalName
\LocalName:
	.long	_SA1Registers + 10
.endm

.macro LOCSYM_SA1XL LocalName
\LocalName:
	.long	_SA1Registers + 10
.endm

.macro LOCSYM_SA1XH LocalName
\LocalName:
	.long	SA1XL + 1
.endm

.macro LOCSYM_SA1YY LocalName
\LocalName:
	.long	_SA1Registers + 12
.endm

.macro LOCSYM_SA1YL LocalName
\LocalName:
	.long	_SA1Registers + 12
.endm

.macro LOCSYM_SA1YH LocalName
\LocalName:
	.long	SA1YL + 1
.endm

.macro LOCSYM_SA1PCR LocalName
\LocalName:
	.long	_SA1Registers + 14
.endm

