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

  S-DD1 decompression code
  (c) Copyright 2003 Brad Jorsch, with many thanks to Andreas Naive



 
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

/* S-DD1 decompressor
 *
 * Based on code and documentation by Andreas Naive, who deserves a great deal
 * of thanks and credit for figuring this out.
 *
 * Andreas says:
 * The author is greatly indebted with The Dumper, without whose help and 
 * patience providing him with real S-DD1 data the research had never been 
 * possible. He also wish to note that in the very beggining of his research,
 * Neviksti had done some steps in the right direction. By last, the author is
 * indirectly indebted to all the people that worked and contributed in the
 * S-DD1 issue in the past.
 */

#include <string.h>
#include "port.h"
#include "sdd1emu.h"

static int valid_bits;
static uint16 in_stream;
static uint8 *in_buf;
static uint8 bit_ctr[8];
static uint8 context_states[32];
static int context_MPS[32];
static int bitplane_type;
static int high_context_bits;
static int low_context_bits;
static int prev_bits[8];

static struct {
    uint8 code_size;
    uint8 MPS_next;
    uint8 LPS_next;
} evolution_table[] = {
    /*  0 */ { 0,25,25},
    /*  1 */ { 0, 2, 1},
    /*  2 */ { 0, 3, 1},
    /*  3 */ { 0, 4, 2},
    /*  4 */ { 0, 5, 3},
    /*  5 */ { 1, 6, 4},
    /*  6 */ { 1, 7, 5},
    /*  7 */ { 1, 8, 6},
    /*  8 */ { 1, 9, 7},
    /*  9 */ { 2,10, 8},
    /* 10 */ { 2,11, 9},
    /* 11 */ { 2,12,10},
    /* 12 */ { 2,13,11},
    /* 13 */ { 3,14,12},
    /* 14 */ { 3,15,13},
    /* 15 */ { 3,16,14},
    /* 16 */ { 3,17,15},
    /* 17 */ { 4,18,16},
    /* 18 */ { 4,19,17},
    /* 19 */ { 5,20,18},
    /* 20 */ { 5,21,19},
    /* 21 */ { 6,22,20},
    /* 22 */ { 6,23,21},
    /* 23 */ { 7,24,22},
    /* 24 */ { 7,24,23},
    /* 25 */ { 0,26, 1},
    /* 26 */ { 1,27, 2},
    /* 27 */ { 2,28, 4},
    /* 28 */ { 3,29, 8},
    /* 29 */ { 4,30,12},
    /* 30 */ { 5,31,16},
    /* 31 */ { 6,32,18},
    /* 32 */ { 7,24,22}
};

static uint8 run_table[128] = {
    128,  64,  96,  32, 112,  48,  80,  16, 120,  56,  88,  24, 104,  40,  72,
      8, 124,  60,  92,  28, 108,  44,  76,  12, 116,  52,  84,  20, 100,  36,
     68,   4, 126,  62,  94,  30, 110,  46,  78,  14, 118,  54,  86,  22, 102,
     38,  70,   6, 122,  58,  90,  26, 106,  42,  74,  10, 114,  50,  82,  18,
     98,  34,  66,   2, 127,  63,  95,  31, 111,  47,  79,  15, 119,  55,  87,
     23, 103,  39,  71,   7, 123,  59,  91,  27, 107,  43,  75,  11, 115,  51,
     83,  19,  99,  35,  67,   3, 125,  61,  93,  29, 109,  45,  77,  13, 117,
     53,  85,  21, 101,  37,  69,   5, 121,  57,  89,  25, 105,  41,  73,   9,
    113,  49,  81,  17,  97,  33,  65,   1
};

static SCHERZO_INLINE uint8 GetCodeword(int bits){
    uint8 tmp;

    if(!valid_bits){
        in_stream|=*(in_buf++);
        valid_bits=8;
    }
    in_stream<<=1;
    valid_bits--;
    in_stream^=0x8000;
    if(in_stream&0x8000) return 0x80+(1<<bits);
    tmp=(in_stream>>8) | (0x7f>>bits);
    in_stream<<=bits;
    valid_bits-=bits;
    if(valid_bits<0){
        in_stream |= (*(in_buf++))<<(-valid_bits);
        valid_bits+=8;
    }
    return run_table[tmp];
}

static SCHERZO_INLINE uint8 GolombGetBit(int code_size){
    if(!bit_ctr[code_size]) bit_ctr[code_size]=GetCodeword(code_size);
    bit_ctr[code_size]--;
    if(bit_ctr[code_size]==0x80){
        bit_ctr[code_size]=0;
        return 2; /* secret code for 'last zero'. ones are always last. */
    }
    return (bit_ctr[code_size]==0)?1:0;
}

static SCHERZO_INLINE uint8 ProbGetBit(uint8 context){
    uint8 state=context_states[context];
    uint8 bit=GolombGetBit(evolution_table[state].code_size);

    if(bit&1){
        context_states[context]=evolution_table[state].LPS_next;
        if(state<2){
            context_MPS[context]^=1;
            return context_MPS[context]; /* just inverted, so just return it */
        } else{
            return context_MPS[context]^1; /* we know bit is 1, so use a constant */
        }
    } else if(bit){
        context_states[context]=evolution_table[state].MPS_next;
        /* zero here, zero there, no difference so drop through. */
    }
    return context_MPS[context]; /* we know bit is 0, so don't bother xoring */
}

static SCHERZO_INLINE uint8 GetBit(uint8 cur_bitplane){
    uint8 bit;
    
    bit=ProbGetBit(((cur_bitplane&1)<<4)
                   | ((prev_bits[cur_bitplane]&high_context_bits)>>5)
                   | (prev_bits[cur_bitplane]&low_context_bits));

    prev_bits[cur_bitplane] <<= 1;
    prev_bits[cur_bitplane] |= bit;
    return bit;
}

void SDD1_decompress(uint8 *out, uint8 *in, int len){
    uint8 bit, i, plane;
    uint8 byte1, byte2;

    if(len==0) len=0x10000;
    
    bitplane_type=in[0]>>6;

    switch(in[0]&0x30){
      case 0x00:
        high_context_bits=0x01c0;
        low_context_bits =0x0001;
        break;
      case 0x10:
        high_context_bits=0x0180;
        low_context_bits =0x0001;
        break;
      case 0x20:
        high_context_bits=0x00c0;
        low_context_bits =0x0001;
        break;
      case 0x30:
        high_context_bits=0x0180;
        low_context_bits =0x0003;
        break;
    }

    in_stream=(in[0]<<11) | (in[1]<<3);
    valid_bits=5;
    in_buf=in+2;
    memset(bit_ctr, 0, sizeof(bit_ctr));
    memset(context_states, 0, sizeof(context_states));
    memset(context_MPS, 0, sizeof(context_MPS));
    memset(prev_bits, 0, sizeof(prev_bits));

    switch(bitplane_type){
      case 0:
        while(1) {
            for(byte1=byte2=0, bit=0x80; bit; bit>>=1){
                if(GetBit(0)) byte1 |= bit;
                if(GetBit(1)) byte2 |= bit;
            }
            *(out++)=byte1;
            if(!--len) return;
            *(out++)=byte2;
            if(!--len) return;
        }
        break;
      case 1:
        i=plane=0;
        while(1) {
            for(byte1=byte2=0, bit=0x80; bit; bit>>=1){
                if(GetBit(plane)) byte1 |= bit;
                if(GetBit(plane+1)) byte2 |= bit;
            }
            *(out++)=byte1;
            if(!--len) return;
            *(out++)=byte2;
            if(!--len) return;
            if(!(i+=32)) plane = (plane+2)&7;
        }
        break;
      case 2:
        i=plane=0;
        while(1) {
            for(byte1=byte2=0, bit=0x80; bit; bit>>=1){
                if(GetBit(plane)) byte1 |= bit;
                if(GetBit(plane+1)) byte2 |= bit;
            }
            *(out++)=byte1;
            if(!--len) return;
            *(out++)=byte2;
            if(!--len) return;
            if(!(i+=32)) plane ^= 2;
        }
        break;
      case 3:
        do {
            for(byte1=plane=0, bit=1; bit; bit<<=1, plane++){
                if(GetBit(plane)) byte1 |= bit;
            }
            *(out++)=byte1;
        } while(--len);
        break;
    }
}

static uint8 cur_plane;
static uint8 num_bits;
static uint8 next_byte;

void SDD1_init(uint8 *in){
    bitplane_type=in[0]>>6;

    switch(in[0]&0x30){
      case 0x00:
        high_context_bits=0x01c0;
        low_context_bits =0x0001;
        break;
      case 0x10:
        high_context_bits=0x0180;
        low_context_bits =0x0001;
        break;
      case 0x20:
        high_context_bits=0x00c0;
        low_context_bits =0x0001;
        break;
      case 0x30:
        high_context_bits=0x0180;
        low_context_bits =0x0003;
        break;
    }

    in_stream=(in[0]<<11) | (in[1]<<3);
    valid_bits=5;
    in_buf=in+2;
    memset(bit_ctr, 0, sizeof(bit_ctr));
    memset(context_states, 0, sizeof(context_states));
    memset(context_MPS, 0, sizeof(context_MPS));
    memset(prev_bits, 0, sizeof(prev_bits));

    cur_plane=0;
    num_bits=0;
}

uint8 SDD1_get_byte(void){
    uint8 bit;
    uint8 byte=0;
    
    switch(bitplane_type){
      case 0:
        num_bits+=16;
        if(num_bits&16){
            next_byte=0;
            for(bit=0x80; bit; bit>>=1){
                if(GetBit(0)) byte |= bit;
                if(GetBit(1)) next_byte |= bit;
            }
            return byte;
        } else {
            return next_byte;
        }

      case 1:
        num_bits+=16;
        if(num_bits&16){
            next_byte=0;
            for(bit=0x80; bit; bit>>=1){
                if(GetBit(cur_plane)) byte |= bit;
                if(GetBit(cur_plane+1)) next_byte |= bit;
            }
            return byte;
        } else {
            if(!num_bits) cur_plane = (cur_plane+2)&7;
            return next_byte;
        }

      case 2:
        num_bits+=16;
        if(num_bits&16){
            next_byte=0;
            for(bit=0x80; bit; bit>>=1){
                if(GetBit(cur_plane)) byte |= bit;
                if(GetBit(cur_plane+1)) next_byte |= bit;
            }
            return byte;
        } else {
            if(!num_bits) cur_plane ^= 2;
            return next_byte;
        }

      case 3:
        for(cur_plane=0, bit=1; bit; bit<<=1, cur_plane++){
            if(GetBit(cur_plane)) byte |= bit;
        }
        return byte;

      default:
        /* should never happen */
        return 0;
    }
}

