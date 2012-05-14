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

CC=gcc-avr
CFLAGS=-Wall -g -c -std=c99 -Os -fpic -DPIC -I$(INCLUDE)

install:
	avrdude -p m32u4 -P COM3 -c avr109

main: $(BIN)/main

$(BIN)/main:
	$(MKDIR) $(BIN)

clean:
	$(RMDIR) $(BIN)
	$(RMDIR) $(OBJ)
	$(RMDIR) $(LIB)

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

installdep:
	$(INSTALL) avr-libc gcc-avr avrdude doxygen binutils-avr

doc:
	doxygen
