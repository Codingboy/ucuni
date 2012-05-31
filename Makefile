SRC=src
INCLUDE=include
BIN=bin
OBJ=obj
LIB=lib

RMDIR=rm -rf
RM=rm -f
CP=cp -f
CPDIR=cp -rf
ECHO=@echo
MV=mv -f
MKDIR=mkdir -p
INSTALL=apt-get install -y
CONTROLLER=-mmcu=atmega32u4
CC=avr-gcc
MODULES_=main pin pinoperations typedefs
MODULES=$(addsuffix .o, $(addprefix $(OBJ)/, $(MODULES_)))
CFLAGS=-Wall -g -c -std=c99 -Os -fpic -DPIC -I$(INCLUDE) $(CONTROLLER) -DF_CPU=16000000

install: $(BIN)/main.hex
	avrdude -p m32u4 -P /dev/ttyACM0 -c avr109 -U application:w:$<:i

main: $(BIN)/main.hex

$(BIN)/main.elf: $(MODULES)
	$(MKDIR) $(BIN)
	$(CC) $(CONTROLLER) -o $@ $^

$(BIN)/main.hex: $(BIN)/main.elf
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

doc:
	doxygen
