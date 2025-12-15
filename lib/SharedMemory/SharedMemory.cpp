#include <Arduino.h>
#include <SharedMemory.h>

alignas(8) uint8_t static_memory_buffer[SHARED_BUFFER_SIZE];
