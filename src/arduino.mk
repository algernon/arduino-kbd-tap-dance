BOARD    						= model01
MCU									= atmega32u4

KBDIO_PATH					= ${HOME}/src/ext/keyboardio-hw
ARDUINO_PATH				= ${HOME}/install/arduino
ARDUINO_TOOLS_PATH	= $(ARDUINO_PATH)/hardware/tools
FQBN								= keyboardio:avr:model01
BUILD_PATH				 := $(shell mktemp -d 2>/dev/null || mktemp -d -t 'build')
OUTPUT_PATH					= ../arduino
ARDUINO_IDE_VERSION	= 100607
SKETCH							= arduino-kbd-tap-dance.ino

AVR_SIZE						= $(ARDUINO_TOOLS_PATH)/avr/bin/avr-size

OUTPUT_FILE_PREFIX	= $(SKETCH)-$(GIT_VERSION)

HEX_FILE_PATH				= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).hex
ELF_FILE_PATH				= $(OUTPUT_PATH)/$(OUTPUT_FILE_PREFIX).elf

all: build
build: compile size

${OUTPUT_PATH}:
	install -d $@

compile: ${OUTPUT_PATH}
	$(ARDUINO_PATH)/arduino-builder \
		-hardware $(ARDUINO_PATH)/hardware \
    -hardware ${KBDIO_PATH}/hardware \
		-tools $(ARDUINO_TOOLS_PATH) \
		-tools $(ARDUINO_PATH)/tools-builder  \
		-fqbn $(FQBN) \
    ${VERBOSE} \
		-build-path $(BUILD_PATH) \
		-ide-version $(ARDUINO_IDE_VERSION) \
		$(SKETCH)
	@cp $(BUILD_PATH)/$(SKETCH).hex $(HEX_FILE_PATH)
	@cp $(BUILD_PATH)/$(SKETCH).elf $(ELF_FILE_PATH)

size: compile
	$(AVR_SIZE) -C --mcu=$(MCU) $(ELF_FILE_PATH)

.PHONY: all build compile size
