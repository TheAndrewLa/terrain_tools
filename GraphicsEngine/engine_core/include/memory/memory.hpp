#ifndef ENGINE_CORE_MEMORY_H
#define ENGINE_CORE_MEMORY_H

#include <types.hpp>

#include <stdexcept>

namespace ala::memory {

template <typename AllocT>
concept allocator_concept =
    requires(AllocT& allocator, types::usize size, types::usize alignment, void* ptr) {
      { allocator.allocate(size) } -> std::convertible_to<void*>;
      { allocator.allocate_aligned(size, alignment) } -> std::convertible_to<void*>;
      { allocator.deallocate(ptr) } noexcept -> std::convertible_to<void>;
    };

class allocation_error final : public std::bad_alloc {
  public:
  allocation_error(const char* message);

  allocation_error(const std::string& message);
  allocation_error(std::string&& message);

  const char* what() const noexcept override;

  private:
  std::string msg_;
};

class allocator {
  public:
  virtual void* allocate(types::usize size) = 0;
  virtual void* allocate_aligned(types::usize size, types::usize alignment) = 0;

  virtual void deallocate(void* ptr) noexcept = 0;
};

class libc_allocator final : public allocator {
  public:
  void* allocate(types::usize size);
  void* allocate_aligned(types::usize size, types::usize alignment);

  void deallocate(void* ptr) noexcept;

  static inline std::shared_ptr<allocator> get() { return std::make_shared<libc_allocator>(); }
};

class linear_allocator final : public allocator {
  public:
  explicit linear_allocator(types::usize initial_space);
  linear_allocator(const linear_allocator& other) = delete;
  linear_allocator(linear_allocator&& other);

  ~linear_allocator();

  linear_allocator& operator=(const linear_allocator& other) = delete;
  linear_allocator& operator=(linear_allocator&& other);

  void* allocate(types::usize size);
  void* allocate_aligned(types::usize size, types::usize alignment);

  void deallocate(void* ptr) noexcept;

  private:
  types::uint8* arena_;
  libc_allocator base_allocator_;
};

class stack_allocator final : public allocator {
  public:
  private:
  types::uint8* arena_;
  libc_allocator base_allocator_;
};

}  // namespace ala::memory

#endif
