#include <Arduino.h>
#include <Utils.h>

void print_and_send_solution(int day, int part, long long result,
                             unsigned long duration) {
#ifdef USE_FIFO_OUTPUT
  rp2040.fifo.push(day);
  rp2040.fifo.push(part);
  rp2040.fifo.push(duration);
  uint32_t high = (uint32_t)(result >> 32);
  rp2040.fifo.push(high);
  uint32_t low = (uint32_t)(result & 0xFFFFFFFF);
  rp2040.fifo.push(low);
#else
  Serial.print("Day ");
  Serial.print(day);
  Serial.print(" Part ");
  Serial.print(part);
  Serial.print(" Solution: ");
  Serial.print(result);
  Serial.printf(" --- Time: %lu ms\n", duration);
#endif
}
