PROG?=apu2
AVRDUDE=sudo avrdude1 -c ${PROG} -p m${MEGA}
F_OSC=16000000

GOOD_MEGA := 0

CFLAGS += --max_errors 1

ifeq "${MEGA}" "164p"
  CFLAGS += -D M164=1
  GOOD_MEGA := 1
endif
ifeq "${MEGA}" "324pa"
  CFLAGS += -D M324=1
  GOOD_MEGA := 1
endif
ifeq "${GOOD_MEGA}" "0"
  $(info > Please provide valid MEGA env variable)
  $(info > Usage:)
  $(info >     MEGA=164p  make ...)
  $(info >     MEGA=324pa make ...)
  $(error No MCU selected!)
endif


flash: build/main_${MEGA}.hex
	${AVRDUDE} -U flash:w:$<:i

fuse:
	${AVRDUDE} -B 1200 -e -U efuse:w:0xfc:m -U hfuse:w:0xc9:m -U lfuse:w:0xd7:m

build/main.hex: build/main.asm *.asm *.inc Makefile | build/.
	cd build; avra ${CFLAGS} -o ../$@ -I ../ main.asm
# 	cd build; avra -fI -D F_OSC=${F_OSC} -I ../ -o $@ main.asm

build/main_${MEGA}.hex: build/main.hex | build/main.asm
	rm build/main.asm
	cp $< $@

build/main.asm: main.asm | build/.
	cp $< $@


build/.:
	mkdir build

