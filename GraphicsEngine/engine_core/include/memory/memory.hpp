#ifndef ENGINE_CORE_MEMORY_H
#define ENGINE_CORE_MEMORY_H

#include "../types.hpp"

#include <concepts>

namespace ala::memory {

template <typename AllocT>
concept comptime_allocator =
    requires(AllocT& allocator, typename AllocT::ptr_t pointer, types::usize size, types::usize alignment) {
      { allocator.allocate(size) } -> std::convertible_to<typename AllocT::ptr_t>;
      { allocator.allocate_aligned(size, alignment) } -> std::convertible_to<typename AllocT::ptr_t>;
      { allocator.deallocate(pointer) } noexcept -> std::convertible_to<void>;
    };

class polymorphic_allocator {
  public:
  virtual void* allocate(types::usize size) = 0;
  virtual void* allocate_aligned(types::usize size, types::usize alignment) = 0;

  virtual void deallocate(void* ptr) noexcept = 0;
};

class libc_allocator : public polymorphic_allocator {
  public:
  void* allocate(types::usize size) final;
  void* allocate_aligned(types::usize size, types::usize alignment) final;

  void deallocate(void* ptr) noexcept final;
};

}  // namespace ala::memory

#endif
