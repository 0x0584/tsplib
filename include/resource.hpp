#pragma once

#include <memory>
#include <memory_resource>

namespace tsp {
struct resource {
  using pointer = std::unique_ptr<resource>;
  using pack = std::unique_ptr<pointer[]>;

  explicit resource(std::size_t size)
      : size_(size), buffer(std::make_unique<std::byte[]>(size)),
        pool_(buffer.get(), size, std::pmr::null_memory_resource()) {}

  static std::size_t size(pointer &ptr) { return ptr->size_; }
  static std::pmr::monotonic_buffer_resource &pool(pointer &ptr) {
    return ptr->pool_;
  }

  static std::size_t size(pack &pack, int idx) { return size(pack[idx]); }
  static std::pmr::monotonic_buffer_resource &pool(pack &pack, int idx) {
    return pool(pack[idx]);
  }

private:
  std::size_t size_;
  std::unique_ptr<std::byte[]> buffer;
  std::pmr::monotonic_buffer_resource pool_;
};
} // namespace tsp