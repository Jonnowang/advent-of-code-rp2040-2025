# Advent of Code 2025 - on an MCU

This repository contains solutions to Advent of Code 2025 implemented for the RP2040 MCU. Each day's solution is organized into its own library.

## Hardware: Arduino Nano RP2040 Connect

This project targets the **Arduino Nano RP2040 Connect**.

*   **MCU:** Raspberry Pi RP2040 (Dual-core Arm Cortex-M0+ @ 133MHz)
*   **Memory:** 264KB SRAM
*   **Storage:** 16MB Flash

## Project Setup

*   **Framework:** Arduino (Earle Philhower core)
*   **Dependencies:** Embedded Template Library (ETL)
*   **Structure:** Solutions are modularized by day in the `lib/` directory. Inputs are stored in headers to avoid filesystem dependencies. LittleFS is optionally usable for a lightweight FS in flash.
