# Dual-Sensor Speed Trap System

Embedded C speed-measurement prototype built around an AVR microcontroller, dual optical sensors, and a hardware feedback interface.

---

## Demo

[Download Demo Video](https://github.com/isaismarrufo/dual-sensor-speed-trap/raw/refs/heads/main/media/demo.mp4)

Visual-only recording of the optical sensors, timing indicator, and LCD in operation.

<img src="media/hardware.jpg" alt="Speed-trap hardware prototype with optical sensors, controls, and LCD" width="360" />

---

## Overview

Developed from October to December 2025, this project combines interrupt-driven sensing, hardware timers, and direct register control to measure the travel time of an object between two optical sensors. The interface presents elapsed time and speed, lets the user adjust speed limits, and provides visual and audible feedback.

The project focuses on low-level firmware, peripheral integration, and hardware/software debugging on a physical embedded system.

---

## Features

- Dual optical sensors for object detection and elapsed-time measurement
- Timer and interrupt-based measurement control with timeout handling
- LCD interface for timing, speed, and limit settings
- Rotary encoder and buttons for adjusting the speed range
- EEPROM storage for settings across power cycles
- Servo indicator, RGB status LED, and buzzer feedback
- Integer arithmetic for speed calculations
- Modular C firmware organized by application, interrupts, initialization, and device control

---

## Hardware

- ATmega328P AVR microcontroller on an Arduino Uno platform
- Two optical sensing pairs using LEDs and phototransistors
- Character LCD
- Rotary encoder and selection buttons
- Servo motor
- RGB LED and timing indicator
- Buzzer
- Breadboard, resistors, and jumper wiring

---

## Engineering Focus

- Embedded C and direct access to microcontroller registers
- GPIO, timer interrupts, and pin-change interrupts
- PWM and peripheral timing
- Nonvolatile settings and user input handling
- Multi-file builds with the AVR toolchain and Makefiles
- Hardware debugging using an oscilloscope and digital multimeter

---

## Tools Used

- C
- AVR-GCC and AVR-LibC
- Make and AVRDUDE
- Oscilloscope and digital multimeter
- Git and GitHub

---

## Repository Contents

```text
src/            Original application, interrupt, control, and ADC source
include/        Application headers and LCD integration interface
media/          Hardware photo and visual-only demo
Makefile        Public-module compilation and optional firmware build
README.md       Project overview and engineering highlights
```

## Key Modules

- `src/project.c` - Main loop, speed calculations, display updates, and status feedback
- `src/interrupt.c` - Optical sensor timing, encoder input, buttons, timeout, and buzzer interrupts
- `src/ports.c` - GPIO, timer, EEPROM, and peripheral initialization
- `src/drivers.c` - Servo position mapping and buzzer note control
- `src/adc.c` - Reused ADC helper; the speed-trap measurement path uses digital sensor inputs and does not call it
- `include/lcd.h` - Integration interface for a separately supplied LCD driver

The nine original application and helper source/header files are published without functional changes, with their existing comments preserved. The original LCD driver and header, course Makefile, assignment worksheets, and test archives remain in a separate private repository. This repository provides a new build file and LCD interface declarations; it does not include the LCD implementation.

---

## Build

Install Make and an AVR toolchain with AVR-GCC, AVR-LibC, `avr-objcopy`, and `avr-size`.

Compile the public modules for an ATmega328P running at 16 MHz:

```sh
make check
```

This checks compilation of the public source files without linking an LCD driver. To build complete firmware, supply a compatible LCD implementation separately:

```sh
make firmware LCD_SOURCE=/path/to/lcd.c
```

The driver must implement the functions in `include/lcd.h` for the original hardware wiring. The resulting `build/main.elf` and `build/main.hex` are local build outputs. The public source alone is not a complete linked firmware image. These targets do not program hardware.

```sh
make clean
```

## Implementation Notes

The source preserves the historical firmware. AVR-GCC reports an overflow in the speed calculation's integer constant expression and format warnings where the LCD output uses `%d` with 32-bit values. Timer2 channel selection and the servo's compare-register use also need review against the physical wiring. Compilation was checked; the hardware was not revalidated for this publication.
