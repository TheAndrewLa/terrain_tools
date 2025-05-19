#include <memory/memory.hpp>

#include <format>

namespace ala::memory {

allocation_error::allocation_error(const std::string& message) : msg_(message) {}

allocation_error::allocation_error(std::string&& message) : msg_(message) {}

allocation_error::allocation_error(const char* message) : msg_(message) {}

const char* allocation_error::what() const noexcept {
  return msg_.c_str();
}

void* libc_allocator::allocate(types::usize size) {
  auto* ret = ::operator new(size, std::nothrow);

  if (ret == nullptr) {
    throw allocation_error{
        std::format("Failed to allocate {} bytes of memory!\nAddress of allocator object: {:#010x}", size,
                    reinterpret_cast<types::uintptr>(this))};
  }

  return ret;
}

void* libc_allocator::allocate_aligned(types::usize size, types::usize alignment) {
  auto* ret = ::operator new(size, std::align_val_t{alignment}, std::nothrow);

  if (ret == nullptr) {
    throw allocation_error{
        std::format("Failed to allocate {} bytes of memory with {}-byte alignment!\nAddress of allocator "
                    "object: {:#010x}",
                    size, alignment, reinterpret_cast<types::uintptr>(this))};
  }

  return ret;
}

void libc_allocator::deallocate(void* ptr) noexcept {
  ::operator delete(ptr);
}

}  // namespace ala::memory
