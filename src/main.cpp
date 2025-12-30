#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <SPI.h>
#include <SharedMemory.h>
#include <Wire.h>

#include <Day1.h>
#include <Day10.h>
#include <Day11.h>
#include <Day12.h>
#include <Day2.h>
#include <Day3.h>
#include <Day4.h>
#include <Day5.h>
#include <Day6.h>
#include <Day7.h>
#include <Day8.h>
#include <Day9.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Serial.begin(115200);
  // while (!Serial) {
  //   delay(100);
  //   Serial.println("Serial is ready!");
  // }

  solve_day1();
  solve_day2();
  solve_day3();
  solve_day4();
  solve_day5();
  solve_day6();
  solve_day7();
  solve_day8();
  solve_day9();
  solve_day10();
  solve_day11();
  solve_day12();
}

void loop() {}

void setup1() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    while (true)
      ;
  }

  display.clearDisplay();

  display.fillRect(0, 0, 128, 16, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setCursor(2, 4);
  display.setTextSize(1);
  display.print("Advent of Code 2025!");

  display.display();
  delay(2500);
}

void loop1() {
  if (rp2040.fifo.available() >= 5) {
    uint32_t day = rp2040.fifo.pop();
    uint32_t part = rp2040.fifo.pop();
    uint32_t duration = rp2040.fifo.pop();
    uint32_t res_high = rp2040.fifo.pop();
    uint32_t res_low = rp2040.fifo.pop();

    long long result = ((long long)res_high << 32) | res_low;

    display.clearDisplay();

    display.fillRect(0, 0, 128, 16, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.setCursor(2, 4);
    display.setTextSize(1);
    display.print("Day ");
    display.print(day);
    display.print(" Part ");
    display.print(part);

    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(0, 25);

    if (result > 9999999999LL) { // > 10 digits
      display.setTextSize(1);
    } else {
      display.setTextSize(2);
    }
    display.print(result);

    // Compute Time
    display.setTextSize(1);
    display.setCursor(0, 50);
    display.print("CPU Time: ");
    display.print(duration);
    display.print(" ms");

    display.display();
  }
  delay(2500);
}
