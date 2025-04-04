# Project name
PROJECT = Dashboard_firmware

# Target microcontroller
MCU = PIC18F47Q84

# Compiler
CC = xc8-CC

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source Files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/CAN.c $(SRC_DIR)/i2c.c $(SRC_DIR)/LCD.c $(SRC_DIR)/Seg.c #$(SRC_DIR)/animation.c

# Output file
OUTPUT = $(BUILD_DIR)/$(PROJECT).hex

# Compiler flags
CFLAGS = -mcpu=$(MCU) -O2 -Wall -I$(INC_DIR)

# Default target
all: $(OUTPUT)

# Rule to build the .hex file
$(OUTPUT): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o$(OUTPUT) $(SOURCES)

# Clean up
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean