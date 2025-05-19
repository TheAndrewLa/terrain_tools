#ifndef ENGINE_CORE_MEMORY_OBJECT_POOL_H
#define ENGINE_CORE_MEMORY_OBJECT_POOL_H

#include <memory/memory.hpp>
#include <types.hpp>

#include <memory>

namespace ala::memory {

class pool {
  public:
  virtual void* get() = 0;
  virtual void release(void* ptr) noexcept = 0;

  virtual void clear() noexcept = 0;

  inline types::usize object_size() const noexcept { return size_; }

  protected:
  pool(types::usize object_size, std::shared_ptr<allocator> allocator = libc_allocator::get());

  types::usize size_;
  std::shared_ptr<allocator> allocator_;
};

template <typename T, typename PoolT = pool>
T* get_from_pool(std::shared_ptr<PoolT> pool) {
  if (pool->object_size() != sizeof(T)) {
    throw std::invalid_argument{"This pool is incompatible with provided type!"};
  }

  return reinterpret_cast<T*>(pool->get());
}

class static_pool final : public pool {};

class static_pool_thread_safe final : public pool {};

class dynamic_pool final : public pool {};

class dynamic_pool_thread_safe final : public pool {};

}  // namespace ala::memory

#endif
