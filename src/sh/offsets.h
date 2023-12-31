//SCPUState
#define Flags @r12
#define BranchSkip @(4,r12)
#define NMIActive @(5,r12)
#define IRQActive @(6,r12)
#define WaitingForInterrupt @(7,r12)
#define InDMA @(8,r12)
#define WhichEvent @(9,r12)
#define SRAMModified @(10,r12)
#define BRKTriggered @(11,r12)
#define PCS @(12,r12)
#define PCBase @(16,r12)
#define PCAtOpcodeStart @(20,r12)
#define WaitAddress @(24,r12)
#define WaitCounter @(28,r12)
#define Cycles @(32,r12)
#define NextEvent @(36,r12)
#define V_Counter @(40,r12)
#define MemSpeed @(44,r12)
#define MemSpeedx2 @(48,r12)
#define FastROMSpeed @(52,r12)
#define NMICycleCount @(56,r12)
#define IRQCycleCount @(60,r12)
#define AutoSaveTimer @(64,r12)
#define NMITriggerPoint @(68,r12)
#define TriedInterleavedMode2 @(72,r12)

//SRegisters
#define PB @r11
#define DB @(1,r11)
#define PP @(2,r11)
#define PL @(2,r11)
#define PH @(3,r11)
#define AA @(4,r11)
#define AL @(4,r11)
#define AH @(5,r11)
#define DD @(6,r11)
#define DL @(6,r11)
#define DH @(7,r11)
#define SS @(8,r11)
#define SL @(8,r11)
#define SH @(9,r11)
#define XX @(10,r11)
#define XL @(10,r11)
#define XH @(11,r11)
#define YY @(12,r11)
#define YL @(12,r11)
#define YH @(13,r11)
#define PCR @(14,r11)

//CMemory
#define RAM _Memory + 0
#define ROM _Memory + 4
#define VRAM _Memory + 8
#define SRAM _Memory + 12
#define BWRAM _Memory + 16
#define FillRAM _Memory + 20
#define C4RAM _Memory + 24
#define HiROM _Memory + 28
#define LoROM _Memory + 29
#define SRAMMask _Memory + 30
#define SRAMSize _Memory + 32
#define Map _Memory + 36
#define WriteMap _Memory + 16420
#define MemorySpeed _Memory + 32804
#define BlockIsRAM _Memory + 36900
#define BlockIsROM _Memory + 40996
#define ROMFilename _Memory + 53364

//SIAPU
#define APUExecuting @r1
#define APUBit @(1,r1)
#define APU_Carry @(2,r1)
#define APU_Zero @(3,r1)
#define APU_Overflow @(4,r1)
#define APUPCS @(8,r1)
#define APURAM @(12,r1)
#define APUDirectPage @(16,r1)
#define APUAddress @(20,r1)
#define APUWaitAddress1 @(24,r1)
#define APUWaitAddress2 @(28,r1)
#define APUWaitCounter @(32,r1)
#define APUShadowRAM @(36,r1)
#define APUCachedSamples @(40,r1)
#define APUTimerErrorCounter @(44,r1)

//SAPU
#define APUCycles @r1
#define APUShowROM @(4,r1)
#define APUFlags @(5,r1)
#define APUKeyedChannels @(6,r1)
#define APUOutPorts @(7,r1)
#define APUDSP @(11,r1)
#define APUExtraRAM 139
#define APUTimer 204
#define APUTimerTarget 210
#define APUTimerEnabled 216
#define TimerValueWritten 219

//SICPU
#define CPUSpeed @r1
#define CPUOpcodes @(4,r1)
#define _Carry @(8,r1)
#define _Zero @(9,r1)
#define _Negative @(10,r1)
#define _Overflow @(11,r1)
#define CPUExecuting @(12,r1)
#define ShiftedPB @(16,r1)
#define ShiftedDB @(20,r1)
#define Frame @(24,r1)
#define Scanline @(28,r1)

//SSettings
#define APUEnabled @r1
#define Shutdown @(1,r1)
#define SoundSkipMethod @(2,r1)
#define PAL @(3,r1)
#define H_Max @(4,r1)
#define HBlankStart @(8,r1)
#define CyclesPercentage @(12,r1)
#define DisableIRQ @(16,r1)
#define Paused @(17,r1)
#define SuperFXEnabled @(80,r1)
#define SA1Enabled @(82,r1)
#define SoundSync @(108,r1)

//SAPURegisters
#define ApuP @r1
#define ApuYA @(2,r1)
#define ApuA @(2,r1)
#define ApuY @(3,r1)
#define ApuX @(4,r1)
#define ApuS @(5,r1)
#define ApuPC @(6,r1)
#define APUPCR @(6,r1)

//SPPU
#define BGMode @r1
#define BG3Priority @(1,r1)
#define Brightness @(2,r1)
#define OAMFlip @(3,r1)
#define OAMAddr @(4,r1)
#define SavedOAMAddr @(6,r1)
#define ScreenHeight @(8,r1)
#define FirstSprite @(10,r1)
#define HVBeamCounterLatched @(11,r1)
#define VTimerEnabled @(12,r1)
#define HTimerEnabled @(13,r1)
#define ForcedBlanking @(14,r1)
#define IRQVBeamPos @(16,r1)
#define HTimerPosition @(18,r1)
#define GHight @(20,r1)
#define GInc @(21,r1)
#define GAddress @(22,r1)
#define GMask1 @(24,r1)
#define GFullGraphicCount @(26,r1)
#define GShift @(28,r1)
#define CGFLIP @(78,r1)
#define CGDATA @(80,r1)
#define LastSprite @(592,r1)
#define OBJ @(594,r1)
#define OAMPriorityRotation @(2130,r1)
#define OAMTileAddress @(2132,r1)
#define IRQHBeamPos @(2134,r1)
#define VBeamPosLatched @(2136,r1)
#define HBeamPosLatched @(2138,r1)
#define HBeamFlip @(2140,r1)
#define VBeamFlip @(2141,r1)
#define MatrixA @(2142,r1)
#define MatrixB @(2144,r1)
#define MatrixC @(2146,r1)
#define MatrixD @(2148,r1)
#define CentreX @(2150,r1)
#define CentreY @(2152,r1)
#define Joypad1ButtonReadPos @(2154,r1)
#define Joypad2ButtonReadPos @(2155,r1)
#define CGADD @(2156,r1)
#define FixedColourGreen @(2158,r1)
#define FixedColourRed @(2157,r1)
#define FixedColourBlue @(2159,r1)
#define WRAM @(2160,r1)
#define BG_Forced @(2164,r1)
#define OBJThroughMain @(2165,r1)
#define OBJThroughSub @(2166,r1)
#define OBJSizeSelect @(2167,r1)
#define OBJNameBase @(2168,r1)
#define OAMReadFlip @(2171,r1)
#define OAMData @(2172,r1)
#define Mosaic @(2716,r1)
#define BGMosaic @(2717,r1)
#define Mode7HFlip @(2721,r1)
#define Mode7VFlip @(2722,r1)
#define Mode7Repeat @(2723,r1)
#define Window1Left @(2724,r1)
#define Window1Right @(2725,r1)
#define Window2Left @(2726,r1)
#define Window2Right @(2727,r1)
#define ClipWindowOverlapLogic @(2734,r1)
#define ClipWindow1Enable @(2740,r1)
#define ClipWindow2Enable @(2746,r1)
#define ClipWindow1Inside @(2752,r1)
#define ClipWindow2Inside @(2758,r1)
#define RecomputeClipWindows @(2764,r1)
#define CGFLIPRead @(2765,r1)
#define OBJNameSelect @(2766,r1)
#define Need16x8Mulitply @(2768,r1)
#define Joypad3ButtonReadPos @(2769,r1)
#define MouseSpeed @(2770,r1)

//InternalPPU
#define ColorsChanged @r1
#define HDMA @(1,r1)
#define HDMAStarted @(2,r1)
#define MaxBrightness @(3,r1)
#define LatchedBlanking @(4,r1)
#define OBJChanged @(5,r1)
#define RenderThisFrame @(6,r1)
#define SkippedFrames @(20,r1)
#define FrameSkip @(24,r1)
#define TileCache @(28,r1)
#define TileCached @(40,r1)
#define FirstVRAMRead @(52,r1)
#define LatchedInterlace @(53,r1)
#define DoubleWidthPixels @(54,r1)
#define RenderedScreenHeight @(56,r1)
#define RenderedScreenWidth @(60,r1)
#define Red @(64,r1)
#define Green @(1088,r1)
#define Blue @(2112,r1)
#define XB @(3136,r1)
#define ScreenColors @(3140,r1)
#define PreviousLine @(3652,r1)
#define CurrentLine @(3656,r1)
#define Joypads @(3664,r1)
#define SuperScope @(3684,r1)
#define Mouse @(3688,r1)
#define PrevMouseX @(3696,r1)
#define PrevMouseY @(3704,r1)
#define Clip @(3712,r1)

//SSA1
#define SA1Opcodes @(0,SA1REG)
#define SA1_Carry @(4,SA1REG)
#define SA1_Zero @(5,SA1REG)
#define SA1_Negative @(6,SA1REG)
#define SA1_Overflow @(7,SA1REG)
#define SA1CPUExecuting @(8,SA1REG)
#define SA1Executing @(9,SA1REG)
#define SA1NMIActive @(10,SA1REG)
#define SA1IRQActive @(11,SA1REG)
#define SA1WaitingForInterrupt @(12,SA1REG)
#define SA1ShiftedPB @(16,SA1REG)
#define SA1ShiftedDB @(20,SA1REG)
#define SA1Flags @(24,SA1REG)
#define SA1PCS @(28,SA1REG)
#define SA1PCBase @(32,SA1REG)
#define SA1BWRAM @(36,SA1REG)
#define SA1PCAtOpcodeStart @(40,SA1REG)
#define SA1WaitAddress @(44,SA1REG)
#define SA1WaitCounter @(48,SA1REG)
#define SA1WaitByteAddress1 @(52,SA1REG)
#define SA1WaitByteAddress2 @(56,SA1REG)
#define SA1Map @(60,SA1REG)
#define SA1WriteMap @(16444,SA1REG)
#define SA1op1 @(32828,SA1REG)
#define SA1op2 @(32830,SA1REG)
#define SA1arithmetic_op @(32832,SA1REG)
#define SA1sum @(32836,SA1REG)
#define SA1overflow @(32844,SA1REG)
#define VirtualBitmapFormat @(32845,SA1REG)
#define SA1_in_char_dma @(32846,SA1REG)
#define SA1variable_bit_pos @(32847,SA1REG)

//SSA1Registers
#define SA1PB @r11
#define SA1DB @(1,r11)
#define SA1PP @(2,r11)
#define SA1PL @(2,r11)
#define SA1PH SA1PL + 1
#define SA1AA @(4,r11)
#define SA1AL @(4,r11)
#define SA1AH SA1AL + 1
#define SA1DD @(6,r11)
#define SA1DL @(6,r11)
#define SA1DH SA1DL + 1
#define SA1SS @(8,r11)
#define SA1SL @(8,r11)
#define SA1SH SA1SL + 1
#define SA1XX @(10,r11)
#define SA1XL @(10,r11)
#define SA1XH SA1XL + 1
#define SA1YY @(12,r11)
#define SA1YL @(12,r11)
#define SA1YH SA1YL + 1
#define SA1PCR @(14,r11)

//Other stuff
#define S9xGetByte @(4,r14)
#define S9xSetByte @(8,r14)
#define S9xGetWord @(12,r14)
#define S9xSetWord @(16,r14)
#define S9xOpcode_IRQ @(20,r14)
#define S9xOpcode_NMI @(24,r14)
#define S9xSetPCBase @(28,r14)
#define ICPU @(32,r14)
#define IAPU @(36,r14)
#define APU @(40,r14)
#define SA1 @(44,r14)
#define APURegisters @(48,r14)
#define Settings @(52,r14)
#define IPPU @(56,r14)
#define PPU @(60,r14)

#define SA1Map_offs 60
#define SA1WriteMap_offs 16444
#define NMITriggerPoint_offs 68




