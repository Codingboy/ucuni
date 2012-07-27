SRC=src
INCLUDE=include
BIN=bin
OBJ=obj
LIB=lib
EXTLIB=extlib
LUFA_PATH=./$(EXTLIB)/LUFA-120219/LUFA
DOC=doc

MCU=atmega32u4
ARCH=AVR8
BOARD=USBKEY
F_CPU=16000000
F_USB=$(F_CPU)
F_CLOCK=$(F_CPU)

RMDIR=rm -rf
RM=rm -f
CP=cp -f
CPDIR=cp -rf
ECHO=@echo
MV=mv -f
MKDIR=mkdir -p
INSTALL=apt-get install -y
CONTROLLER=-mmcu=$(MCU)
CC=avr-gcc
MODULES_=pin pinoperations typedefs led button time usb globals descriptors ez3 servo temperature
MODULES=$(addsuffix .o, $(addprefix $(OBJ)/, $(MODULES_)))
CFLAGS=-Wall -g -c -std=c99 -Os -fpic -DPIC -I$(INCLUDE) -I$(LUFA_PATH)/Drivers/USB -I$(LUFA_PATH)/Drivers/USB/Core/AVR8 -I$(LUFA_PATH) $(CONTROLLER) -DF_CPU=$(F_CPU) -DF_USB=$(F_USB) -DMCU=$(MCU) -DARCH=$(ARCH) -DBOARD=$(BOARD) -DF_CLOCK=$(F_CLOCK) -DUSE_FLASH_DESCRIPTORS -DUSE_STATIC_OPTIONS="(USE_DEVICE_OPT_FULLSPEED | USB_OPT_AUTO_PLL)" -DUSB_DEVICE_ONLY

installdemo: $(BIN)/demo.hex
	avrdude -p m32u4 -P /dev/ttyACM0 -c avr109 -U flash:w:$<:i

installmain: $(BIN)/main.hex
	avrdude -p m32u4 -P /dev/ttyACM0 -c avr109 -U flash:w:$<:i

main: $(BIN)/main.hex

demo: $(BIN)/demo.hex

lufa: $(LIB)/liblufa.a

$(LIB)/liblufa.a:
	$(MKDIR) $(LIB)
	$(CP) $(EXTLIB)/LUFA-120219/Demos/Device/LowLevel/Keyboard/libKeyboard.a $@

$(BIN)/main.elf: $(MODULES) $(OBJ)/main.o
	make lufa
	$(MKDIR) $(BIN)
	$(CC) $(CONTROLLER) -o $@ $^ -L$(LIB) -llufa

$(BIN)/demo.elf: $(MODULES) $(OBJ)/demo.o
	make lufa
	$(MKDIR) $(BIN)
	$(CC) $(CONTROLLER) -o $@ $^ -L$(LIB) -llufa

$(BIN)/main.hex: $(BIN)/main.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

$(BIN)/demo.hex: $(BIN)/demo.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

clean:
	$(RMDIR) $(BIN)
	$(RMDIR) $(OBJ)
	$(RMDIR) $(LIB)

$(OBJ)/%.o: $(SRC)/%.c
	$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

installdep:
	$(INSTALL) avr-libc gcc-avr avrdude doxygen binutils-avr

doku: $(DOC)
	doxygen

$(DOC):
	doku
