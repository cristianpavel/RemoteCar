SOURCES = ./sources
OBJ_DIR = ./obj
CC = avr-g++
CFLAGS = -mmcu=atmega324p -DF_CPU=16000000 -O3 -Wall
OBJCOPY = avr-objcopy
BOOTLOADER = ./bootloadHID
OBJECTS = $(OBJ_DIR)/l298n.o $(OBJ_DIR)/timer1.o $(OBJ_DIR)/hcsr04.o $(OBJ_DIR)/usart.o

all: $(OBJ_DIR)/main.hex

$(OBJ_DIR)/main.elf: $(SOURCES)/main.c $(OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/l298n.o: $(SOURCES)/l298n.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/timer1.o: $(SOURCES)/timer1.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/hcsr04.o: $(SOURCES)/hcsr04.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/usart.o: $(SOURCES)/usart.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/main.hex: $(OBJ_DIR)/main.elf
	$(OBJCOPY) -j .text -j .data -O ihex $^ $@

upload: $(OBJ_DIR)/main.hex
	$(BOOTLOADER) -r $^

clean:
	rm -rf $(OBJ_DIR)/*

.PHONY: all clean upload
