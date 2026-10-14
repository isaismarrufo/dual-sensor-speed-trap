AVR_CC ?= avr-gcc
AVR_OBJCOPY ?= avr-objcopy
AVR_SIZE ?= avr-size
MCU ?= atmega328p
F_CPU ?= 16000000UL

CPPFLAGS += -Iinclude -DF_CPU=$(F_CPU)
CFLAGS += -mmcu=$(MCU) -Os -Wall
SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c,build/%.o,$(SOURCES))

.DEFAULT_GOAL := check
.PHONY: check firmware clean

check: $(OBJECTS)

build:
	mkdir -p $@

build/%.o: src/%.c | build
	$(AVR_CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

# LCD_SOURCE points to a separately supplied, compatible LCD implementation.
firmware: check
	@test -n "$(LCD_SOURCE)" || { echo "Set LCD_SOURCE to the path of a compatible lcd.c driver."; exit 1; }
	$(AVR_CC) $(CPPFLAGS) $(CFLAGS) -c "$(LCD_SOURCE)" -o build/lcd.o
	$(AVR_CC) $(CFLAGS) $(OBJECTS) build/lcd.o -o build/main.elf
	$(AVR_OBJCOPY) -j .text -j .data -O ihex build/main.elf build/main.hex
	$(AVR_SIZE) --format=avr --mcu=$(MCU) build/main.elf

clean:
	rm -rf build

-include $(OBJECTS:.o=.d)
