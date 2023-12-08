#include "offsets.h"

.macro LOCSYM_Flags LocalName
\LocalName:
	.long	OFFSET_Flags
.endm

.macro LOCSYM_BranchSkip LocalName
\LocalName:
	.long	OFFSET_BranchSkip
.endm

.macro LOCSYM_NMIActive LocalName
\LocalName:
	.long	OFFSET_NMIActive
.endm

.macro LOCSYM_IRQActive LocalName
\LocalName:
	.long	OFFSET_IRQActive
.endm

.macro LOCSYM_WaitingForInterrupt LocalName
\LocalName:
	.long	OFFSET_WaitingForInterrupt
.endm

.macro LOCSYM_InDMA LocalName
\LocalName:
	.long	OFFSET_InDMA
.endm

.macro LOCSYM_WhichEvent LocalName
\LocalName:
	.long	OFFSET_WhichEvent
.endm

.macro LOCSYM_PCS LocalName
\LocalName:
	.long	OFFSET_PCS
.endm

.macro LOCSYM_PCBase LocalName
\LocalName:
	.long	OFFSET_PCBase
.endm

.macro LOCSYM_PCAtOpcodeStart LocalName
\LocalName:
	.long	OFFSET_PCAtOpcodeStart
.endm

.macro LOCSYM_WaitAddress LocalName
\LocalName:
	.long	OFFSET_WaitAddress
.endm

.macro LOCSYM_WaitCounter LocalName
\LocalName:
	.long	OFFSET_WaitCounter
.endm

.macro LOCSYM_Cycles LocalName
\LocalName:
	.long	OFFSET_Cycles
.endm

.macro LOCSYM_NextEvent LocalName
\LocalName:
	.long	OFFSET_NextEvent
.endm

.macro LOCSYM_V_Counter LocalName
\LocalName:
	.long	OFFSET_V_Counter
.endm

.macro LOCSYM_MemSpeed LocalName
\LocalName:
	.long	OFFSET_MemSpeed
.endm

.macro LOCSYM_MemSpeedx2 LocalName
\LocalName:
	.long	OFFSET_MemSpeedx2
.endm

.macro LOCSYM_FastROMSpeed LocalName
\LocalName:
	.long	OFFSET_FastROMSpeed
.endm

.macro LOCSYM_AutoSaveTimer LocalName
\LocalName:
	.long	OFFSET_AutoSaveTimer
.endm

.macro LOCSYM_SRAMModified LocalName
\LocalName:
	.long	OFFSET_SRAMModified
.endm

.macro LOCSYM_NMITriggerPoint LocalName
\LocalName:
	.long	OFFSET_NMITriggerPoint
.endm

.macro LOCSYM_TriedInterleavedMode2 LocalName
\LocalName:
	.long	OFFSET_TriedInterleavedMode2
.endm

.macro LOCSYM_BRKTriggered LocalName
\LocalName:
	.long	OFFSET_BRKTriggered
.endm

.macro LOCSYM_NMICycleCount LocalName
\LocalName:
	.long	OFFSET_NMICycleCount
.endm

.macro LOCSYM_IRQCycleCount LocalName
\LocalName:
	.long	OFFSET_IRQCycleCount
.endm

.macro LOCSYM_PB LocalName
\LocalName:
	.long	OFFSET_PB
.endm

.macro LOCSYM_DB LocalName
\LocalName:
	.long	OFFSET_DB
.endm

.macro LOCSYM_PP LocalName
\LocalName:
	.long	OFFSET_PP
.endm

.macro LOCSYM_PL LocalName
\LocalName:
	.long	OFFSET_PL
.endm

.macro LOCSYM_PH LocalName
\LocalName:
	.long	OFFSET_PH
.endm

.macro LOCSYM_AA LocalName
\LocalName:
	.long	OFFSET_AA
.endm

.macro LOCSYM_AL LocalName
\LocalName:
	.long	OFFSET_AL
.endm

.macro LOCSYM_AH LocalName
\LocalName:
	.long	OFFSET_AH
.endm

.macro LOCSYM_DD LocalName
\LocalName:
	.long	OFFSET_DD
.endm

.macro LOCSYM_DL LocalName
\LocalName:
	.long	OFFSET_DL
.endm

.macro LOCSYM_DH LocalName
\LocalName:
	.long	OFFSET_DH
.endm

.macro LOCSYM_SS LocalName
\LocalName:
	.long	OFFSET_SS
.endm

.macro LOCSYM_SL LocalName
\LocalName:
	.long	OFFSET_SL
.endm

.macro LOCSYM_SH LocalName
\LocalName:
	.long	OFFSET_SH
.endm

.macro LOCSYM_XX LocalName
\LocalName:
	.long	OFFSET_XX
.endm

.macro LOCSYM_XL LocalName
\LocalName:
	.long	OFFSET_XL
.endm

.macro LOCSYM_XH LocalName
\LocalName:
	.long	OFFSET_XH
.endm

.macro LOCSYM_YY LocalName
\LocalName:
	.long	OFFSET_YY
.endm

.macro LOCSYM_YL LocalName
\LocalName:
	.long	OFFSET_YL
.endm

.macro LOCSYM_YH LocalName
\LocalName:
	.long	OFFSET_YH
.endm

.macro LOCSYM_PCR LocalName
\LocalName:
	.long	OFFSET_PCR
.endm

.macro LOCSYM_RAM LocalName
\LocalName:
	.long	OFFSET_RAM
.endm

.macro LOCSYM_ROM LocalName
\LocalName:
	.long	OFFSET_ROM
.endm

.macro LOCSYM_VRAM LocalName
\LocalName:
	.long	OFFSET_VRAM
.endm

.macro LOCSYM_SRAM LocalName
\LocalName:
	.long	OFFSET_SRAM
.endm

.macro LOCSYM_BWRAM LocalName
\LocalName:
	.long	OFFSET_BWRAM
.endm

.macro LOCSYM_FillRAM LocalName
\LocalName:
	.long	OFFSET_FillRAM
.endm

.macro LOCSYM_C4RAM LocalName
\LocalName:
	.long	OFFSET_C4RAM
.endm

.macro LOCSYM_HiROM LocalName
\LocalName:
	.long	OFFSET_HiROM
.endm

.macro LOCSYM_LoROM LocalName
\LocalName:
	.long	OFFSET_LoROM
.endm

.macro LOCSYM_SRAMMask LocalName
\LocalName:
	.long	OFFSET_SRAMMask
.endm

.macro LOCSYM_SRAMSize LocalName
\LocalName:
	.long	OFFSET_SRAMSize
.endm

.macro LOCSYM_Map LocalName
\LocalName:
	.long	OFFSET_Map
.endm

.macro LOCSYM_WriteMap LocalName
\LocalName:
	.long	OFFSET_WriteMap
.endm

.macro LOCSYM_MemorySpeed LocalName
\LocalName:
	.long	OFFSET_MemorySpeed
.endm

.macro LOCSYM_BlockIsRAM LocalName
\LocalName:
	.long	OFFSET_BlockIsRAM
.endm

.macro LOCSYM_BlockIsROM LocalName
\LocalName:
	.long	OFFSET_BlockIsROM
.endm

.macro LOCSYM_ROMFilename LocalName
\LocalName:
	.long	OFFSET_ROMFilename
.endm

.macro LOCSYM_APUPCS LocalName
\LocalName:
	.long	OFFSET_APUPCS
.endm

.macro LOCSYM_APURAM LocalName
\LocalName:
	.long	OFFSET_APURAM
.endm

.macro LOCSYM_APUExecuting LocalName
\LocalName:
	.long	OFFSET_APUExecuting
.endm

.macro LOCSYM_APUDirectPage LocalName
\LocalName:
	.long	OFFSET_APUDirectPage
.endm

.macro LOCSYM_APUBit LocalName
\LocalName:
	.long	OFFSET_APUBit
.endm

.macro LOCSYM_APUAddress LocalName
\LocalName:
	.long	OFFSET_APUAddress
.endm

.macro LOCSYM_APUWaitAddress1 LocalName
\LocalName:
	.long	OFFSET_APUWaitAddress1
.endm

.macro LOCSYM_APUWaitAddress2 LocalName
\LocalName:
	.long	OFFSET_APUWaitAddress2
.endm

.macro LOCSYM_APUWaitCounter LocalName
\LocalName:
	.long	OFFSET_APUWaitCounter
.endm

.macro LOCSYM_APUShadowRAM LocalName
\LocalName:
	.long	OFFSET_APUShadowRAM
.endm

.macro LOCSYM_APUCachedSamples LocalName
\LocalName:
	.long	OFFSET_APUCachedSamples
.endm

.macro LOCSYM_APU_Carry LocalName
\LocalName:
	.long	OFFSET_APU_Carry
.endm

.macro LOCSYM_APU_Zero LocalName
\LocalName:
	.long	OFFSET_APU_Zero
.endm

.macro LOCSYM_APU_Overflow LocalName
\LocalName:
	.long	OFFSET_APU_Overflow
.endm

.macro LOCSYM_APUTimerErrorCounter LocalName
\LocalName:
	.long	OFFSET_APUTimerErrorCounter
.endm

.macro LOCSYM_APUCycles LocalName
\LocalName:
	.long	OFFSET_APUCycles
.endm

.macro LOCSYM_APUShowROM LocalName
\LocalName:
	.long	OFFSET_APUShowROM
.endm

.macro LOCSYM_APUFlags LocalName
\LocalName:
	.long	OFFSET_APUFlags
.endm

.macro LOCSYM_APUKeyedChannels LocalName
\LocalName:
	.long	OFFSET_APUKeyedChannels
.endm

.macro LOCSYM_APUOutPorts LocalName
\LocalName:
	.long	OFFSET_APUOutPorts
.endm

.macro LOCSYM_APUDSP LocalName
\LocalName:
	.long	OFFSET_APUDSP
.endm

.macro LOCSYM_APUExtraRAM LocalName
\LocalName:
	.long	OFFSET_APUExtraRAM
.endm

.macro LOCSYM_APUTimer LocalName
\LocalName:
	.long	OFFSET_APUTimer
.endm

.macro LOCSYM_APUTimerTarget LocalName
\LocalName:
	.long	OFFSET_APUTimerTarget
.endm

.macro LOCSYM_APUTimerEnabled LocalName
\LocalName:
	.long	OFFSET_APUTimerEnabled
.endm

.macro LOCSYM_TimerValueWritten LocalName
\LocalName:
	.long	OFFSET_TimerValueWritten
.endm

.macro LOCSYM_CPUSpeed LocalName
\LocalName:
	.long	OFFSET_CPUSpeed
.endm

.macro LOCSYM_CPUOpcodes LocalName
\LocalName:
	.long	OFFSET_CPUOpcodes
.endm

.macro LOCSYM__Carry LocalName
\LocalName:
	.long	OFFSET__Carry
.endm

.macro LOCSYM__Zero LocalName
\LocalName:
	.long	OFFSET__Zero
.endm

.macro LOCSYM__Negative LocalName
\LocalName:
	.long	OFFSET__Negative
.endm

.macro LOCSYM__Overflow LocalName
\LocalName:
	.long	OFFSET__Overflow
.endm

.macro LOCSYM_ShiftedDB LocalName
\LocalName:
	.long	OFFSET_ShiftedDB
.endm

.macro LOCSYM_ShiftedPB LocalName
\LocalName:
	.long	OFFSET_ShiftedPB
.endm

.macro LOCSYM_CPUExecuting LocalName
\LocalName:
	.long	OFFSET_CPUExecuting
.endm

.macro LOCSYM_Scanline LocalName
\LocalName:
	.long	OFFSET_Scanline
.endm

.macro LOCSYM_Frame LocalName
\LocalName:
	.long	OFFSET_Frame
.endm

.macro LOCSYM_APUEnabled LocalName
\LocalName:
	.long	OFFSET_APUEnabled
.endm

.macro LOCSYM_Shutdown LocalName
\LocalName:
	.long	OFFSET_Shutdown
.endm

.macro LOCSYM_SoundSkipMethod LocalName
\LocalName:
	.long	OFFSET_SoundSkipMethod
.endm

.macro LOCSYM_H_Max LocalName
\LocalName:
	.long	OFFSET_H_Max
.endm

.macro LOCSYM_HBlankStart LocalName
\LocalName:
	.long	OFFSET_HBlankStart
.endm

.macro LOCSYM_CyclesPercentage LocalName
\LocalName:
	.long	OFFSET_CyclesPercentage
.endm

.macro LOCSYM_DisableIRQ LocalName
\LocalName:
	.long	OFFSET_DisableIRQ
.endm

.macro LOCSYM_Paused LocalName
\LocalName:
	.long	OFFSET_Paused
.endm

.macro LOCSYM_PAL LocalName
\LocalName:
	.long	OFFSET_PAL
.endm

.macro LOCSYM_SoundSync LocalName
\LocalName:
	.long	OFFSET_SoundSync
.endm

.macro LOCSYM_SA1Enabled LocalName
\LocalName:
	.long	OFFSET_SA1Enabled
.endm

.macro LOCSYM_SuperFXEnabled LocalName
\LocalName:
	.long	OFFSET_SuperFXEnabled
.endm

.macro LOCSYM_ApuP LocalName
\LocalName:
	.long	OFFSET_ApuP
.endm

.macro LOCSYM_ApuYA LocalName
\LocalName:
	.long	OFFSET_ApuYA
.endm

.macro LOCSYM_ApuA LocalName
\LocalName:
	.long	OFFSET_ApuA
.endm

.macro LOCSYM_ApuY LocalName
\LocalName:
	.long	OFFSET_ApuY
.endm

.macro LOCSYM_ApuX LocalName
\LocalName:
	.long	OFFSET_ApuX
.endm

.macro LOCSYM_ApuS LocalName
\LocalName:
	.long	OFFSET_ApuS
.endm

.macro LOCSYM_ApuPC LocalName
\LocalName:
	.long	OFFSET_ApuPC
.endm

.macro LOCSYM_APUPCR LocalName
\LocalName:
	.long	OFFSET_APUPCR
.endm

.macro LOCSYM_BGMode LocalName
\LocalName:
	.long	OFFSET_BGMode
.endm

.macro LOCSYM_BG3Priority LocalName
\LocalName:
	.long	OFFSET_BG3Priority
.endm

.macro LOCSYM_Brightness LocalName
\LocalName:
	.long	OFFSET_Brightness
.endm

.macro LOCSYM_GHight LocalName
\LocalName:
	.long	OFFSET_GHight
.endm

.macro LOCSYM_GInc LocalName
\LocalName:
	.long	OFFSET_GInc
.endm

.macro LOCSYM_GAddress LocalName
\LocalName:
	.long	OFFSET_GAddress
.endm

.macro LOCSYM_GMask1 LocalName
\LocalName:
	.long	OFFSET_GMask1
.endm

.macro LOCSYM_GFullGraphicCount LocalName
\LocalName:
	.long	OFFSET_GFullGraphicCount
.endm

.macro LOCSYM_GShift LocalName
\LocalName:
	.long	OFFSET_GShift
.endm

.macro LOCSYM_CGFLIP LocalName
\LocalName:
	.long	OFFSET_CGFLIP
.endm

.macro LOCSYM_CGDATA LocalName
\LocalName:
	.long	OFFSET_CGDATA
.endm

.macro LOCSYM_FirstSprite LocalName
\LocalName:
	.long	OFFSET_FirstSprite
.endm

.macro LOCSYM_LastSprite LocalName
\LocalName:
	.long	OFFSET_LastSprite
.endm

.macro LOCSYM_OBJ LocalName
\LocalName:
	.long	OFFSET_OBJ
.endm

.macro LOCSYM_OAMPriorityRotation LocalName
\LocalName:
	.long	OFFSET_OAMPriorityRotation
.endm

.macro LOCSYM_OAMAddr LocalName
\LocalName:
	.long	OFFSET_OAMAddr
.endm

.macro LOCSYM_OAMFlip LocalName
\LocalName:
	.long	OFFSET_OAMFlip
.endm

.macro LOCSYM_OAMTileAddress LocalName
\LocalName:
	.long	OFFSET_OAMTileAddress
.endm

.macro LOCSYM_IRQVBeamPos LocalName
\LocalName:
	.long	OFFSET_IRQVBeamPos
.endm

.macro LOCSYM_IRQHBeamPos LocalName
\LocalName:
	.long	OFFSET_IRQHBeamPos
.endm

.macro LOCSYM_VBeamPosLatched LocalName
\LocalName:
	.long	OFFSET_VBeamPosLatched
.endm

.macro LOCSYM_HBeamPosLatched LocalName
\LocalName:
	.long	OFFSET_HBeamPosLatched
.endm

.macro LOCSYM_HBeamFlip LocalName
\LocalName:
	.long	OFFSET_HBeamFlip
.endm

.macro LOCSYM_VBeamFlip LocalName
\LocalName:
	.long	OFFSET_VBeamFlip
.endm

.macro LOCSYM_HVBeamCounterLatched LocalName
\LocalName:
	.long	OFFSET_HVBeamCounterLatched
.endm

.macro LOCSYM_MatrixA LocalName
\LocalName:
	.long	OFFSET_MatrixA
.endm

.macro LOCSYM_MatrixB LocalName
\LocalName:
	.long	OFFSET_MatrixB
.endm

.macro LOCSYM_MatrixC LocalName
\LocalName:
	.long	OFFSET_MatrixC
.endm

.macro LOCSYM_MatrixD LocalName
\LocalName:
	.long	OFFSET_MatrixD
.endm

.macro LOCSYM_CentreX LocalName
\LocalName:
	.long	OFFSET_CentreX
.endm

.macro LOCSYM_CentreY LocalName
\LocalName:
	.long	OFFSET_CentreY
.endm

.macro LOCSYM_Joypad1ButtonReadPos LocalName
\LocalName:
	.long	OFFSET_Joypad1ButtonReadPos
.endm

.macro LOCSYM_Joypad2ButtonReadPos LocalName
\LocalName:
	.long	OFFSET_Joypad2ButtonReadPos
.endm

.macro LOCSYM_CGADD LocalName
\LocalName:
	.long	OFFSET_CGADD
.endm

.macro LOCSYM_FixedColourGreen LocalName
\LocalName:
	.long	OFFSET_FixedColourGreen
.endm

.macro LOCSYM_FixedColourRed LocalName
\LocalName:
	.long	OFFSET_FixedColourRed
.endm

.macro LOCSYM_FixedColourBlue LocalName
\LocalName:
	.long	OFFSET_FixedColourBlue
.endm

.macro LOCSYM_SavedOAMAddr LocalName
\LocalName:
	.long	OFFSET_SavedOAMAddr
.endm

.macro LOCSYM_ScreenHeight LocalName
\LocalName:
	.long	OFFSET_ScreenHeight
.endm

.macro LOCSYM_WRAM LocalName
\LocalName:
	.long	OFFSET_WRAM
.endm

.macro LOCSYM_BG_Forced LocalName
\LocalName:
	.long	OFFSET_BG_Forced
.endm

.macro LOCSYM_ForcedBlanking LocalName
\LocalName:
	.long	OFFSET_ForcedBlanking
.endm

.macro LOCSYM_OBJThroughMain LocalName
\LocalName:
	.long	OFFSET_OBJThroughMain
.endm

.macro LOCSYM_OBJThroughSub LocalName
\LocalName:
	.long	OFFSET_OBJThroughSub
.endm

.macro LOCSYM_OBJSizeSelect LocalName
\LocalName:
	.long	OFFSET_OBJSizeSelect
.endm

.macro LOCSYM_OBJNameBase LocalName
\LocalName:
	.long	OFFSET_OBJNameBase
.endm

.macro LOCSYM_OAMReadFlip LocalName
\LocalName:
	.long	OFFSET_OAMReadFlip
.endm

.macro LOCSYM_OAMData LocalName
\LocalName:
	.long	OFFSET_OAMData
.endm

.macro LOCSYM_VTimerEnabled LocalName
\LocalName:
	.long	OFFSET_VTimerEnabled
.endm

.macro LOCSYM_HTimerEnabled LocalName
\LocalName:
	.long	OFFSET_HTimerEnabled
.endm

.macro LOCSYM_HTimerPosition LocalName
\LocalName:
	.long	OFFSET_HTimerPosition
.endm

.macro LOCSYM_Mosaic LocalName
\LocalName:
	.long	OFFSET_Mosaic
.endm

.macro LOCSYM_BGMosaic LocalName
\LocalName:
	.long	OFFSET_BGMosaic
.endm

.macro LOCSYM_Mode7HFlip LocalName
\LocalName:
	.long	OFFSET_Mode7HFlip
.endm

.macro LOCSYM_Mode7VFlip LocalName
\LocalName:
	.long	OFFSET_Mode7VFlip
.endm

.macro LOCSYM_Mode7Repeat LocalName
\LocalName:
	.long	OFFSET_Mode7Repeat
.endm

.macro LOCSYM_Window1Left LocalName
\LocalName:
	.long	OFFSET_Window1Left
.endm

.macro LOCSYM_Window1Right LocalName
\LocalName:
	.long	OFFSET_Window1Right
.endm

.macro LOCSYM_Window2Left LocalName
\LocalName:
	.long	OFFSET_Window2Left
.endm

.macro LOCSYM_Window2Right LocalName
\LocalName:
	.long	OFFSET_Window2Right
.endm

.macro LOCSYM_ClipWindowOverlapLogic LocalName
\LocalName:
	.long	OFFSET_ClipWindowOverlapLogic
.endm

.macro LOCSYM_ClipWindow1Enable LocalName
\LocalName:
	.long	OFFSET_ClipWindow1Enable
.endm

.macro LOCSYM_ClipWindow2Enable LocalName
\LocalName:
	.long	OFFSET_ClipWindow2Enable
.endm

.macro LOCSYM_ClipWindow1Inside LocalName
\LocalName:
	.long	OFFSET_ClipWindow1Inside
.endm

.macro LOCSYM_ClipWindow2Inside LocalName
\LocalName:
	.long	OFFSET_ClipWindow2Inside
.endm

.macro LOCSYM_RecomputeClipWindows LocalName
\LocalName:
	.long	OFFSET_RecomputeClipWindows
.endm

.macro LOCSYM_CGFLIPRead LocalName
\LocalName:
	.long	OFFSET_CGFLIPRead
.endm

.macro LOCSYM_OBJNameSelect LocalName
\LocalName:
	.long	OFFSET_OBJNameSelect
.endm

.macro LOCSYM_Need16x8Mulitply LocalName
\LocalName:
	.long	OFFSET_Need16x8Mulitply
.endm

.macro LOCSYM_Joypad3ButtonReadPos LocalName
\LocalName:
	.long	OFFSET_Joypad3ButtonReadPos
.endm

.macro LOCSYM_MouseSpeed LocalName
\LocalName:
	.long	OFFSET_MouseSpeed
.endm

.macro LOCSYM_RangeTimeOver LocalName
\LocalName:
	.long	OFFSET_RangeTimeOver
.endm

.macro LOCSYM_ColorsChanged LocalName
\LocalName:
	.long	OFFSET_ColorsChanged
.endm

.macro LOCSYM_HDMA LocalName
\LocalName:
	.long	OFFSET_HDMA
.endm

.macro LOCSYM_HDMAStarted LocalName
\LocalName:
	.long	OFFSET_HDMAStarted
.endm

.macro LOCSYM_MaxBrightness LocalName
\LocalName:
	.long	OFFSET_MaxBrightness
.endm

.macro LOCSYM_LatchedBlanking LocalName
\LocalName:
	.long	OFFSET_LatchedBlanking
.endm

.macro LOCSYM_OBJChanged LocalName
\LocalName:
	.long	OFFSET_OBJChanged
.endm

.macro LOCSYM_RenderThisFrame LocalName
\LocalName:
	.long	OFFSET_RenderThisFrame
.endm

.macro LOCSYM_SkippedFrames LocalName
\LocalName:
	.long	OFFSET_SkippedFrames
.endm

.macro LOCSYM_FrameSkip LocalName
\LocalName:
	.long	OFFSET_FrameSkip
.endm

.macro LOCSYM_TileCache LocalName
\LocalName:
	.long	OFFSET_TileCache
.endm

.macro LOCSYM_TileCached LocalName
\LocalName:
	.long	OFFSET_TileCached
.endm

.macro LOCSYM_FirstVRAMRead LocalName
\LocalName:
	.long	OFFSET_FirstVRAMRead
.endm

.macro LOCSYM_Interlace LocalName
\LocalName:
	.long	OFFSET_Interlace
.endm

.macro LOCSYM_DoubleWidthPixels LocalName
\LocalName:
	.long	OFFSET_DoubleWidthPixels
.endm

.macro LOCSYM_RenderedScreenHeight LocalName
\LocalName:
	.long	OFFSET_RenderedScreenHeight
.endm

.macro LOCSYM_RenderedScreenWidth LocalName
\LocalName:
	.long	OFFSET_RenderedScreenWidth
.endm

.macro LOCSYM_Red LocalName
\LocalName:
	.long	OFFSET_Red
.endm

.macro LOCSYM_Green LocalName
\LocalName:
	.long	OFFSET_Green
.endm

.macro LOCSYM_Blue LocalName
\LocalName:
	.long	OFFSET_Blue
.endm

.macro LOCSYM_XB LocalName
\LocalName:
	.long	OFFSET_XB
.endm

.macro LOCSYM_ScreenColors LocalName
\LocalName:
	.long	OFFSET_ScreenColors
.endm

.macro LOCSYM_PreviousLine LocalName
\LocalName:
	.long	OFFSET_PreviousLine
.endm

.macro LOCSYM_CurrentLine LocalName
\LocalName:
	.long	OFFSET_CurrentLine
.endm

.macro LOCSYM_Joypads LocalName
\LocalName:
	.long	OFFSET_Joypads
.endm

.macro LOCSYM_SuperScope LocalName
\LocalName:
	.long	OFFSET_SuperScope
.endm

.macro LOCSYM_Mouse LocalName
\LocalName:
	.long	OFFSET_Mouse
.endm

.macro LOCSYM_PrevMouseX LocalName
\LocalName:
	.long	OFFSET_PrevMouseX
.endm

.macro LOCSYM_PrevMouseY LocalName
\LocalName:
	.long	OFFSET_PrevMouseY
.endm

.macro LOCSYM_Clip LocalName
\LocalName:
	.long	OFFSET_Clip
.endm

.macro LOCSYM_SA1Opcodes LocalName
\LocalName:
	.long	OFFSET_SA1Opcodes
.endm

.macro LOCSYM_SA1_Carry LocalName
\LocalName:
	.long	OFFSET_SA1_Carry
.endm

.macro LOCSYM_SA1_Zero LocalName
\LocalName:
	.long	OFFSET_SA1_Zero
.endm

.macro LOCSYM_SA1_Negative LocalName
\LocalName:
	.long	OFFSET_SA1_Negative
.endm

.macro LOCSYM_SA1_Overflow LocalName
\LocalName:
	.long	OFFSET_SA1_Overflow
.endm

.macro LOCSYM_SA1CPUExecuting LocalName
\LocalName:
	.long	OFFSET_SA1CPUExecuting
.endm

.macro LOCSYM_SA1ShiftedPB LocalName
\LocalName:
	.long	OFFSET_SA1ShiftedPB
.endm

.macro LOCSYM_SA1ShiftedDB LocalName
\LocalName:
	.long	OFFSET_SA1ShiftedDB
.endm

.macro LOCSYM_SA1Flags LocalName
\LocalName:
	.long	OFFSET_SA1Flags
.endm

.macro LOCSYM_SA1Executing LocalName
\LocalName:
	.long	OFFSET_SA1Executing
.endm

.macro LOCSYM_SA1NMIActive LocalName
\LocalName:
	.long	OFFSET_SA1NMIActive
.endm

.macro LOCSYM_SA1IRQActive LocalName
\LocalName:
	.long	OFFSET_SA1IRQActive
.endm

.macro LOCSYM_SA1WaitingForInterrupt LocalName
\LocalName:
	.long	OFFSET_SA1WaitingForInterrupt
.endm

.macro LOCSYM_SA1PCS LocalName
\LocalName:
	.long	OFFSET_SA1PCS
.endm

.macro LOCSYM_SA1PCBase LocalName
\LocalName:
	.long	OFFSET_SA1PCBase
.endm

.macro LOCSYM_SA1PCAtOpcodeStart LocalName
\LocalName:
	.long	OFFSET_SA1PCAtOpcodeStart
.endm

.macro LOCSYM_SA1WaitAddress LocalName
\LocalName:
	.long	OFFSET_SA1WaitAddress
.endm

.macro LOCSYM_SA1WaitCounter LocalName
\LocalName:
	.long	OFFSET_SA1WaitCounter
.endm

.macro LOCSYM_SA1WaitByteAddress1 LocalName
\LocalName:
	.long	OFFSET_SA1WaitByteAddress1
.endm

.macro LOCSYM_SA1WaitByteAddress2 LocalName
\LocalName:
	.long	OFFSET_SA1WaitByteAddress2
.endm

.macro LOCSYM_SA1BWRAM LocalName
\LocalName:
	.long	OFFSET_SA1BWRAM
.endm

.macro LOCSYM_SA1Map LocalName
\LocalName:
	.long	OFFSET_SA1Map
.endm

.macro LOCSYM_SA1WriteMap LocalName
\LocalName:
	.long	OFFSET_SA1WriteMap
.endm

.macro LOCSYM_SA1op1 LocalName
\LocalName:
	.long	OFFSET_SA1op1
.endm

.macro LOCSYM_SA1op2 LocalName
\LocalName:
	.long	OFFSET_SA1op2
.endm

.macro LOCSYM_SA1arithmetic_op LocalName
\LocalName:
	.long	OFFSET_SA1arithmetic_op
.endm

.macro LOCSYM_SA1sum LocalName
\LocalName:
	.long	OFFSET_SA1sum
.endm

.macro LOCSYM_SA1overflow LocalName
\LocalName:
	.long	OFFSET_SA1overflow
.endm

.macro LOCSYM_VirtualBitmapFormat LocalName
\LocalName:
	.long	OFFSET_VirtualBitmapFormat
.endm

.macro LOCSYM_SA1_in_char_dma LocalName
\LocalName:
	.long	OFFSET_SA1_in_char_dma
.endm

.macro LOCSYM_SA1variable_bit_pos LocalName
\LocalName:
	.long	OFFSET_SA1variable_bit_pos
.endm

.macro LOCSYM_SA1PB LocalName
\LocalName:
	.long	OFFSET_SA1PB
.endm

.macro LOCSYM_SA1DB LocalName
\LocalName:
	.long	OFFSET_SA1DB
.endm

.macro LOCSYM_SA1PP LocalName
\LocalName:
	.long	OFFSET_SA1PP
.endm

.macro LOCSYM_SA1PL LocalName
\LocalName:
	.long	OFFSET_SA1PL
.endm

.macro LOCSYM_SA1PH LocalName
\LocalName:
	.long	OFFSET_SA1PH
.endm

.macro LOCSYM_SA1AA LocalName
\LocalName:
	.long	OFFSET_SA1AA
.endm

.macro LOCSYM_SA1AL LocalName
\LocalName:
	.long	OFFSET_SA1AL
.endm

.macro LOCSYM_SA1AH LocalName
\LocalName:
	.long	OFFSET_SA1AH
.endm

.macro LOCSYM_SA1DD LocalName
\LocalName:
	.long	OFFSET_SA1DD
.endm

.macro LOCSYM_SA1DL LocalName
\LocalName:
	.long	OFFSET_SA1DL
.endm

.macro LOCSYM_SA1DH LocalName
\LocalName:
	.long	OFFSET_SA1DH
.endm

.macro LOCSYM_SA1SS LocalName
\LocalName:
	.long	OFFSET_SA1SS
.endm

.macro LOCSYM_SA1SL LocalName
\LocalName:
	.long	OFFSET_SA1SL
.endm

.macro LOCSYM_SA1SH LocalName
\LocalName:
	.long	OFFSET_SA1SH
.endm

.macro LOCSYM_SA1XX LocalName
\LocalName:
	.long	OFFSET_SA1XX
.endm

.macro LOCSYM_SA1XL LocalName
\LocalName:
	.long	OFFSET_SA1XL
.endm

.macro LOCSYM_SA1XH LocalName
\LocalName:
	.long	OFFSET_SA1XH
.endm

.macro LOCSYM_SA1YY LocalName
\LocalName:
	.long	OFFSET_SA1YY
.endm

.macro LOCSYM_SA1YL LocalName
\LocalName:
	.long	OFFSET_SA1YL
.endm

.macro LOCSYM_SA1YH LocalName
\LocalName:
	.long	OFFSET_SA1YH
.endm

.macro LOCSYM_SA1PCR LocalName
\LocalName:
	.long	OFFSET_SA1PCR
.endm
