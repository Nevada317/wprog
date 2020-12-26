PROGRAMMER=apu2
AVRDUDE=sudo avrdude1 -c ${PROGRAMMER} -p m324pa
F_OSC=16000000

flash: build/main.hex
	${AVRDUDE} -U flash:w:$<:i

fuse:
	${AVRDUDE} -B 1200 -e -U efuse:w:0xfc:m -U hfuse:w:0xc9:m -U lfuse:w:0xd7:m

build/main.hex: build/main.asm *.asm *.inc Makefile | build/.
	cd build; avra -o ../$@ -I ../ main.asm
# 	cd build; avra -fI -D F_OSC=${F_OSC} -I ../ -o $@ main.asm


build/main.asm: main.asm | build/.
	cp $< $@


build/.:
	mkdir build

