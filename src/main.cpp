#include <Arduino.h>

#include <Day1.h>

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(100);
    Serial.println("Serial is ready!");
  }

  solve_day1();
}

void loop() {}
