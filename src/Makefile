# Super Famicast Makefile
# by Scherzo

TARGET = superfamicast.elf
OBJS =  utils.o main.o 2xsai.o apu.o apudebug.o c4.o c4emu.o cheats2.o clip.o cpu.o data.o debug.o dma.o 
OBJS += dsp1.o fxdbg.o fxemu.o fxinst.o gfx.o globals.o loadzip.o memmap.o netplay.o obc1.o ppu.o screenshot.o 
OBJS += sdd1.o sdd1emu.o server.o seta010.o seta011.o seta018.o seta.o snaporig.o snapshot.o snes9x.o soundux.o spc700.o 
OBJS += spc7110.o spc.o spccycles.o srtc.o tile.o explode.o unreduce.o unshrink.o unzip.o cheats.o scanf.o dc_utils.o 
OBJS += dc_vmu.o XML.o dc_menu.o pvr_texture.o dc_controller.o dc_mouse.o dc_file_browser.o test.o sa1.o scherzo_snd_stream.o 
OBJS += offsets.o

CPP_CPU_OBJS = cpuexec.o cpuops.o sa1cpu.o 
ASM_CPU_OBJS = sh/cpuexec.o sh/cpuops.o sh/sa1ops.o 

MY_AFLAGS = -Wa,-little -DSPC700_C -DVAR_CYCLES -DCPU_SHUTDOWN -DSPC700_SHUTDOWN

OBJS += $(ASM_CPU_OBJS)

KOS_EXTRA_FLAGS = -D_XBOX -DUNZIP_SUPPORT -DNOASM -DSDD1_DECOMP -DSPC700_C -DVAR_CYCLES -DCPU_SHUTDOWN -DSPC700_SHUTDOWN -funroll-loops 
KOS_CFLAGS += $(KOS_EXTRA_FLAGS)
KOS_CPPFLAGS += $(KOS_EXTRA_FLAGS)

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	-rm -f $(TARGET) $(BIN) $(OBJS) romdisk.*

cleanasm:
	-rm -f $(ASM_CPU_OBJS)

rm-elf:
	-rm -f $(TARGET) $(BIN) romdisk.*

$(TARGET): $(OBJS) romdisk.o
	$(KOS_CC) $(KOS_STARTUP) $(OBJS) romdisk.o -o $(TARGET) $(KOS_LDFLAGS) $(KOS_LIBS) -lmp3 -loggvorbisplay -lparallax -lkmg -lpng -ljpeg -lgl -lm -lkallisti -lz -lm -lgcc -lk++ -lkosutils
	$(KOS_STRIP) $(TARGET)
	$(KOS_OBJCOPY) -O binary $(TARGET) bin/raw.bin
	scramble bin/raw.bin bin/1ST_READ.BIN
	cp bin/1ST_READ.BIN cd/1ST_READ.BIN
	cp $(TARGET) bin/$(TARGET)

romdisk.img:
	$(KOS_GENROMFS) -f romdisk.img -d romdisk -v

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk_boot romdisk.o

sh/cpuexec.o:
	$(KOS_CC) $(MY_AFLAGS) -c sh/cpuexec.S -o sh/cpuexec.o

sh/cpuops.o:
	$(KOS_CC) $(MY_AFLAGS) -c sh/cpuops.S -o sh/cpuops.o

sh/sa1ops.o:
	$(KOS_CC) $(MY_AFLAGS) -c sh/sa1ops.S -o sh/sa1ops.o

cdimg:
	mkisofs -J -o data.iso cd
