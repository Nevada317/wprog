PROGRAMMER=apu2
F_OSC=16000000

build/main.hex: build/main.asm *.asm *.inc Makefile | build/.
	cd build; avra -o ../$@ -I ../ main.asm
# 	cd build; avra -fI -D F_OSC=${F_OSC} -I ../ -o $@ main.asm


build/main.asm: main.asm | build/.
	cp $< $@


build/.:
	mkdir build

