#pragma once

#include <memory>
#include <memory_resource>

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tsp {
struct graph {
  using vertex = std::size_t;

  graph(const graph &) = default;
  graph(graph &&) = default;

  graph &operator=(const graph &) = default;
  graph &operator=(graph &&) = default;

  static graph build(const char *file);

private:
  graph() = default;

  std::unique_ptr<std::byte[]> A_buffer, B_Buffer;
  std::pmr::monotonic_buffer_resource A_pool, B_pool;
  std::pmr::unordered_map<vertex, std::pmr::unordered_set<vertex>> A;
  std::pmr::vector<std::pmr::vector<bool>> B;
};
} // namespace tsp