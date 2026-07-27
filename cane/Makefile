# ==========================
# ESP32 Arduino Makefile
# ==========================

# ----- Configuration -----

BOARD      := esp32:esp32:esp32
SKETCH_DIR := src
PORT       ?= /dev/ttyUSB0
BAUD       := 115200

# Build output directory
BUILD_DIR  := build

# ----- Commands -----

.PHONY: all build upload flash monitor clean fqbn

all: build

build:
	arduino-cli compile \
		--fqbn $(BOARD) \
		--libraies libs \
		--build-path $(BUILD_DIR) \
		--only-compilation-database \
		$(SKETCH_DIR)

upload:
	arduino-cli upload \
		-p $(PORT) \
		--fqbn $(BOARD) \
		$(SKETCH_DIR)

flash: build upload

monitor:
	arduino-cli monitor \
		-p $(PORT) \
		-c baudrate=$(BAUD)

clean:
	rm -rf $(BUILD_DIR)

fqbn:
	arduino-cli board list
