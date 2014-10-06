# GLOBAL VARIABLES
BUILD_HOST=$(shell uname)
LIBDIR = ./bin/lib/
UECC_SOURCE = ./src/uEcc/*.c
MESSAGING_SOURCE = ./src/messaging/*.c
COMMAS_SRC = ./src/commas/*.cpp
INTERMEDATES = ./src/intermediates/
BAUD_RATE = 19200

# AVR VARIABLES
AVRPORT = /dev/ttyUSB1
AVRGPP = /usr/bin/avr-g++
AVRGCC = /usr/share/arduino/hardware/tools/avr/bin/avr-gcc
AVRAR = /usr/bin/avr-ar
AVROBJCOPY = /usr/local/bin/avr-objcopy
AVRSTRIP = /usr/local/bin/avr-strip
AVRSIZE = /usr/local/bin/avr-size
AVRDUDE = /usr/share/arduino/hardware/tools/avrdude

MCU=atmega328p
CFLAGS += -I. -I./include
CPPFLAGS += -mmcu=$(MCU) -DF_CPU=16000000L
TINYSERIAL_OBJECTS = ./src/tinyserial/*.o
AVR_INTERFACE_SRC = ./src/interfaces/AVR/*.cpp
AVR_SCR = ./src/avr/
AVR_SHA_SRC = ./src/sha256/*.S

# UNIX VARIABLES
PORT = /dev/ttyUSB1
KEYS_DIRECTORY = /home/stephen/key
GPP = /usr/bin/g++
AR = /usr/bin/ar
GPPFLAGS += -I. -I./include -Wno-unused-result
BR_SRC = ./src/border_router/
NODE_SRC = ./src/X86_Node/
UNIX_SHA_SRC = ./src/sha256/*.cpp
UNIX_INTERFACE_SRC = ./src/interfaces/Unix/*.cpp
KEY_SRC = ./src/keys/

AVR_FACTORY:
	make clean
	make AVR_LIB
	make FACTORY_ELF
	make HEX

AVR:
	make clean
	make AVR_LIB
	make ELF
	make HEX

X86:
	make clean
	make X86_LIB
	make X86_APP

KEYS:
	make clean
	make X86_LIB
	make KEY_HEADER

clean:
	@echo [92mCleaning Up[0m
	rm -rf bin/*
	rm -f *.o *.a *.map $(INTERMEDATES)*.o

AVR_LIB:
	@echo [92mMaking the Library for AVR[0m
	$(AVRGCC) $(CFLAGS) $(CPPFLAGS) -std=gnu99 -Os -Wall -c $(AVR_SHA_SRC)
	$(AVRGCC) $(CFLAGS) $(CPPFLAGS) -fPIC -O1 -DuECC_CURVE=uECC_secp256r1 -DuECC_ASM=uECC_asm_small -DuECC_PLATFORM=uECC_avr -c $(UECC_SOURCE)
	$(AVRGCC) $(CFLAGS) $(CPPFLAGS) -c -Os $(MESSAGING_SOURCE)
	$(AVRGCC) $(CFLAGS) $(CPPFLAGS) -DBAUD_RATE=$(BAUD_RATE) -c -Os $(AVR_INTERFACE_SRC)
	$(AVRGPP) $(CFLAGS) $(CPPFLAGS) -ffunction-sections -fdata-sections -g -Os -w -fno-exceptions -c $(COMMAS_SRC)
	mkdir -p $(INTERMEDATES)
	mkdir -p $(LIBDIR)
	mv ./*.o $(INTERMEDATES)
	cp src/tinyserial/libtinyserial.a $(LIBDIR)
	cp $(TINYSERIAL_OBJECTS) $(INTERMEDATES)
	$(AVRAR) rcs $(LIBDIR)libCommas.a $(INTERMEDATES)*.o

X86_LIB:
	@echo [92mMaking the Library for X86[0m
	$(GPP) $(GPPFLAGS)  -Os -Wall -c $(UNIX_SHA_SRC)
	$(GPP) $(GPPFLAGS) -c -Os $(MESSAGING_SOURCE)
	$(GPP) $(GPPFLAGS) -c -O1 -DuECC_CURVE=uECC_secp256r1  $(UECC_SOURCE)
	$(GPP) $(GPPFLAGS) -c -Os -Dport='"'$(PORT)'"' -Ddirectory='"'$(KEYS_DIRECTORY)'"' $(UNIX_INTERFACE_SRC)
	$(GPP) $(GPPFLAGS) -c $(COMMAS_SRC)
	mkdir -p $(INTERMEDATES)
	mkdir -p $(LIBDIR)
	mkdir -p $(KEYS_DIRECTORY)
	mv ./*.o $(INTERMEDATES)
	$(AR) rcs $(LIBDIR)libCommas.a $(INTERMEDATES)*.o

X86_APP:
	@echo [92mCreating binary for X86[0m
	$(GPP) $(GPPFLAGS) -c $(BR_SRC)border_router.cpp
	mv ./*.o $(INTERMEDATES)
	$(GPP) $(INTERMEDATES)border_router.o -L$(LIBDIR) -lCommas -o bin/border_router

X86_NODE:
	@echo [92mCreating binary for X86 Endpoint[0m
	$(GPP) $(GPPFLAGS) -c $(NODE_SRC)node.cpp
	$(GPP) node.o -L$(LIBDIR) -lCommas -o bin/node

KEY_HEADER:
	@echo [92mCreating keys.h header file[0m
	./$(KEY_SRC)makeKeys.sh
	$(GPP) $(GPPFLAGS) -c $(KEY_SRC)gen_key_header.cpp
	mv ./*.o $(INTERMEDATES)
	$(GPP) $(INTERMEDATES)gen_key_header.o -L$(LIBDIR) -lCommas -o bin/gen_key_header
	./bin/gen_key_header
	rm -f temp_key_file.h
	mv keys.h ./include/

FACTORY_ELF:
	$(AVRGCC) $(CFLAGS) $(CPPFLAGS) -Wall -Os -o bin/firmware.elf $(AVR_SCR)main_factory.cpp $(LIBDIR)libCommas.a $(LIBDIR)libtinyserial.a
	$(AVRSTRIP) bin/firmware.elf

ELF:
	@echo [92mLinking firmware.elf[0m
	$(AVRGCC) $(CFLAGS) $(CPPFLAGS) -Wall -Os -o bin/firmware.elf $(AVR_SCR)main.cpp $(LIBDIR)libCommas.a $(LIBDIR)libtinyserial.a
	$(AVRSTRIP) bin/firmware.elf

HEX:
	@echo [92mConverting to firmware.hex[0m
	$(AVROBJCOPY) -O ihex -R .eeprom bin/firmware.elf bin/firmware.hex
	$(AVRSIZE) bin/firmware.elf

UPLOAD:
	$(AVRDUDE) -patmega328p -b115200 -carduino -P$(AVRPORT) -D -Uflash:w:bin/firmware.hex:i