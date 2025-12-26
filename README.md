# Advent of Code 2025 - on an MCU

This repository contains solutions to Advent of Code 2025 implemented for the RP2040 MCU. Each day's solution is organized into its own library.

## Usage Stats

Final *static* memory usage (mostly tried to statically allocate all require objects):

```
RAM:   [=====     ]  53.6% (used 140616 bytes from 262144 bytes)
Flash: [          ]   0.6% (used 98736 bytes from 16773120 bytes)

   text    data     bss     dec     hex
 421332   12032  138300  571664   8b910
```

Final program execution times (only used one out of the two Cortex cores):

```
Day 1 Part 1 Solution: XYZ --- Time: 27 ms
Day 1 Part 2 Solution: XYZ --- Time: 28 ms
Day 2 Part 1 Solution: XYZ --- Time: 2 ms
Day 2 Part 2 Solution: XYZ --- Time: 29845 ms
Day 3 Part 1 Solution: XYZ --- Time: 13 ms
Day 3 Part 2 Solution: XYZ --- Time: 17 ms
Day 4 Part 1 Solution: XYZ --- Time: 42 ms
Day 4 Part 2 Solution: XYZ --- Time: 623 ms
Day 5 Part 1 Solution: XYZ --- Time: 57 ms
Day 5 Part 2 Solution: XYZ --- Time: 12 ms
Day 6 Part 1 Solution: XYZ --- Time: 32 ms
Day 6 Part 2 Solution: XYZ --- Time: 38 ms
Day 7 Part 1 Solution: XYZ --- Time: 110 ms
Day 7 Part 2 Solution: XYZ --- Time: 411 ms
Day 8 Part 1 Solution: XYZ --- Time: 992 ms
Day 8 Part 2 Solution: XYZ --- Time: 2111 ms
Day 9 Part 1 Solution: XYZ --- Time: 132 ms
Day 9 Part 2 Solution: XYZ --- Time: 8686 ms
Day 10 Part 1 Solution: XYZ --- Time: 167 ms
Day 10 Part 2 Solution: XYZ --- Time: 163358 ms
Day 11 Part 1 Solution: XYZ --- Time: 47 ms
Day 11 Part 2 Solution: XYZ --- Time: 3029 ms
Day 12 Solution: 595 --- Time: 53 ms
```

## Hardware: Arduino Nano RP2040 Connect

This project targets the **Arduino Nano RP2040 Connect**.

*   **MCU:** Raspberry Pi RP2040 (Dual-core Arm Cortex-M0+ @ 133MHz)
*   **Memory:** 264KB SRAM
*   **Storage:** 16MB Flash

## Project Setup

*   **Framework:** Arduino (Earle Philhower core)
*   **Dependencies:** Embedded Template Library (ETL)
*   **Structure:** Solutions are modularized by day in the `lib/` directory. Inputs are stored in headers to avoid filesystem dependencies. LittleFS is optionally usable for a lightweight FS in flash.
