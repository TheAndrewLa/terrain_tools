#ifndef ENGINE_CORE_MEMORY_OBJECT_POOL_H
#define ENGINE_CORE_MEMORY_OBJECT_POOL_H

#include <memory/memory.hpp>

namespace ala::memory {

// Default object pool is not thread safe, there's specialized version of thread safe object pool
// TODO: think about polymorphic pool

class object_pool {
  protected:
  using byte = types::uint8;

  public:
  object_pool(polymorphic_allocator& allocator, types::usize object_size, types::usize max_count);
  object_pool(const object_pool&) = delete;
  object_pool(object_pool&& pool);

  ~object_pool();

  object_pool& operator=(const object_pool&) = delete;
  object_pool& operator=(object_pool&& pool);

  template <typename T>
  T* allocate_object() {}

  template <typename T, typename... Args>
  T* construct_object(Args&&... arguments) {}

  void free_object(void* ptr) noexcept;

  protected:
  byte* pool_ptr_;
  byte* free_ptr_;

  types::usize object_size_;
  types::usize pool_size_;
};

class object_pool_thread_safe : object_pool {
	// TODO: describe
};

}  // namespace ala::memory

#endif
