#include <Arduino.h>

#include <Day1.h>
#include <Day2.h>
#include <Day3.h>
#include <Day4.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
    Serial.println("Serial is ready!");
  }

  // solve_day1();
  // solve_day2();
  // solve_day3();
  solve_day4();
  Serial.println("Done!");
}

void loop() {}
