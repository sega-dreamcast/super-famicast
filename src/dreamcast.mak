all-before:
	rm -f romdisk.img romdisk.o
	genromfs -f romdisk.img -d romdisk -v
	bin2o romdisk.img romdisk_boot romdisk.o
	
all-after : 
#	dc-tool -e -x $(BIN)
	sh-elf-gcc startup.o example.o 2xsai.o apu.o apudebug.o c4.o c4emu.o cheats2.o clip.o cpu.o cpuexec.o cpuops.o data.o debug.o dma.o dsp1.o fxdbg.o fxemu.o fxinst.o gfx.o globals.o loadzip.o memmap.o netplay.o obc1.o ppu.o sa1.o sa1cpu.o screenshot.o sdd1.o sdd1emu.o server.o seta010.o seta011.o seta018.o seta.o snaporig.o snapshot.o snes9x.o soundux.o spc700.o spc7110.o spc.o spccycles.o srtc.o tile.o explode.o unreduce.o unshrink.o unzip.o cheats.o scanf.o dc_utils.o dc_vmu.o XML.o dc_menu.o pvr_texture.o dc_controller.o dc_file_browser.o -o $(BIN) -L"C:/Dev-Cpp/dclib" -ml -m4-single-only -nostartfiles -nostdlib -Wl,-Ttext=0x8c010000 romdisk.o -lmp3 -loggvorbisplay -lparallax -lkmg -lpng -ljpeg -lkglx -lkallisti -lz -lm -lgcc -lk++
	sh-elf-strip $(BIN)
	sh-elf-objcopy.exe -O binary $(BIN) bin/raw.bin
	scramble bin/raw.bin bin/1ST_READ.BIN
	copy bin\1ST_READ.BIN cd\1ST_READ.BIN
	copy $(BIN) bin\$(BIN)
	mkisofs -J -o data.iso cd

