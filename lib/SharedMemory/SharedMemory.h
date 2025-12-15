#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include <new>
#include <utility>

constexpr size_t SHARED_BUFFER_SIZE = 96 * 1024; // 96KB

extern uint8_t static_memory_buffer[SHARED_BUFFER_SIZE];

template <typename T>
class StaticMemoryBuffer {
public:
  template <typename... Args>
  StaticMemoryBuffer(Args&&... args) {
    instance = new (static_memory_buffer) T(std::forward<Args>(args)...);
  }

  ~StaticMemoryBuffer() {
    instance->~T();
  }

  T* operator->() { return instance; }
  T& operator*() { return *instance; }
  const T* operator->() const { return instance; }
  const T& operator*() const { return *instance; }

private:
  T* instance;
};

#endif
