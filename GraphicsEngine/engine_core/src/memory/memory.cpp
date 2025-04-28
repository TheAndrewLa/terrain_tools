#include <memory/memory.hpp>

#include <cstdlib>
#include <stdexcept>

namespace ala::memory {

void* libc_allocator::allocate(types::usize size) {
  auto* result = std::malloc(size);

  if (result == nullptr) {
    throw std::bad_alloc("Failure in libc allocator!");
  }

  return result;
}

void* libc_allocator::allocate_aligned(types::usize size, types::usize alignment) {
  auto* result = std::aligned_alloc(alignment, size);

  if (result == nullptr) {
    throw std::bad_alloc("Failure in libc allocator!");
  }

  return result;
}

void libc_allocator::deallocate(void* ptr) noexcept {
  std::free(ptr);
}

}  // namespace ala::memory
