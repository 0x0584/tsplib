#pragma once

#include <memory>
#include <memory_resource>

namespace tsp {
struct resource {
  using pointer = std::unique_ptr<resource>;
  using pack = std::unique_ptr<pointer[]>;

  explicit resource(std::size_t size)
      : buffer(std::make_unique<std::byte[]>(size)),
        pool(buffer.get(), size, std::pmr::null_memory_resource()) {}
        
  std::pmr::monotonic_buffer_resource &get() { return pool; }

private:
  std::unique_ptr<std::byte[]> buffer;
  std::pmr::monotonic_buffer_resource pool;
};
} // namespace tsp