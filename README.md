# PIC-Smart-Meter-ADC
# Smart Energy Meter Front-End (PIC16F877A)

A bare-metal C firmware implementation of a smart meter front-end, simulating real-time metrology data processing.

## Overview
This project reads simulated analog voltage and current sensors, calculates total apparent power, and drives a 16x2 alphanumeric LCD. It was developed using the CCS C Compiler and verified via hardware simulation in Proteus.

## Technical Highlights
* **Custom LCD Driver:** Developed a standalone 4-bit LCD initialization and control sequence. This bypassed standard high-level libraries (`lcd.c`) to resolve critical timing desynchronization and strict busy-flag polling issues between the 16MHz microcontroller and the 250kHz LCD controller.
* **ADC Configuration:** Configured the PIC16F877A's 10-bit ADC using internal VDD/VSS references and a divided clock (ADC_CLOCK_DIV_32) to ensure sufficient capacitor charge time for accurate multi-channel sampling.
* **Real-Time Processing:** Implemented a continuous polling loop to scale raw 0-1023 ADC values into 0-250V and 0-50A ranges, executing real-time power (Wattage) calculations.

## Files Included
* `main.c`: Core firmware, custom LCD driver, and ADC logic.
* `simulation.pdsprj`: Proteus schematic.
* `hardware_sim.png`: Screenshot of the active simulation calculating live power.
