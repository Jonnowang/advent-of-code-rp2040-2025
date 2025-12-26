#include <Arduino.h>
#include <SharedMemory.h>

#include <Day1.h>
#include <Day2.h>
#include <Day3.h>
#include <Day4.h>
#include <Day5.h>
#include <Day6.h>
#include <Day7.h>
#include <Day8.h>
#include <Day9.h>
#include <Day10.h>
#include <Day11.h>
#include <Day12.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
    Serial.println("Serial is ready!");
  }

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
  Serial.println("Done!");
}

void loop() {}
