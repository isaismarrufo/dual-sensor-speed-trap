# Dual-Sensor Speed Trap System

An ATmega328P speed trap with two optical sensors, an LCD, and adjustable speed limits. Written in C using AVR registers, timers, and interrupts.

## Demo

[Download Demo Video](https://github.com/isaismarrufo/dual-sensor-speed-trap/raw/refs/heads/main/media/demo.mp4)

The video shows the sensors, timing indicator, and LCD in operation. It has no audio.

<img src="media/hardware.jpg" alt="Speed-trap hardware prototype with optical sensors, controls, and LCD" width="360" />

## Overview

Built from October to December 2025. The firmware times an object between two optical sensors, calculates its speed, and displays both values on the LCD. A rotary encoder and buttons set the speed limits, which are saved in EEPROM. A servo, RGB LED, and buzzer provide feedback.

## Features

- Interrupt-driven sensor timing with timeout handling
- LCD display for elapsed time, speed, and limit settings
- Adjustable limits retained across power cycles
- Servo position, RGB LED status, and buzzer alerts
- Integer arithmetic for speed calculations

## Hardware

- ATmega328P on an Arduino Uno, running at 16 MHz
- Two optical sensors using LEDs and phototransistors
- Character LCD
- Rotary encoder and selection buttons
- Servo motor
- RGB LED and timing indicator
- Buzzer
- Breadboard, resistors, and jumper wiring

## Tools Used

- C
- AVR-GCC and AVR-LibC
- Make and AVRDUDE
- Oscilloscope and digital multimeter
- Git and GitHub

## Repository Contents

```text
src/            C source files
include/        Headers
media/          Hardware photo and demo video
Makefile        Build targets
```

## Key Modules

- `src/project.c` - Main loop, speed calculation, LCD updates, and feedback
- `src/interrupt.c` - Sensor timing, encoder, button, timeout, and buzzer interrupts
- `src/ports.c` - GPIO, timer, EEPROM, and peripheral setup
- `src/drivers.c` - Servo mapping and buzzer control
- `src/adc.c` - ADC helper included in the project; the speed trap does not use it
- `include/lcd.h` - Function declarations for the LCD driver

## Build

Install Make and an AVR toolchain with AVR-GCC, AVR-LibC, `avr-objcopy`, and `avr-size`.

Compile the source files for an ATmega328P at 16 MHz:

```sh
make check
```

This checks compilation without linking. For a complete build, supply an LCD driver that matches `include/lcd.h` and the circuit wiring:

```sh
make firmware LCD_SOURCE=/path/to/lcd.c
```

The firmware build produces `build/main.elf` and `build/main.hex`. To remove build outputs:

```sh
make clean
```
