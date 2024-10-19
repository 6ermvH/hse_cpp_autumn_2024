#include "allocator.hpp"

Allocator::Allocator() : memory_(nullptr), reserve_(0), capacity_(0) {}

Allocator::~Allocator() { delete[] memory_; }

void Allocator::makeAllocator(size_t maxSize) {
  if (memory_ != nullptr) {
    delete[] memory_;
    reserve_ = 0;
  }
  memory_ = new char[maxSize];
  capacity_ = maxSize;
}

char* Allocator::alloc(size_t size) {
  if ((capacity_ - reserve_) < size) {
    return nullptr;
  } else {
    char* allocated = memory_ + reserve_;
    reserve_ += size;
    return allocated;
  }
}

void Allocator::reset() { reserve_ = 0; }
