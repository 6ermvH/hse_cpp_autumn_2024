#pragma once
#include <cstddef>

class Allocator {
 public:
  Allocator();
  ~Allocator();

  void makeAllocator(size_t maxSize);
  char* alloc(size_t size);
  void reset();

 private:
  char* memory_;
  size_t reserve_;
  size_t capacity_;
};
