#pragma once

#include <memory>
#include <memory_resource>

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tsp {
extern const std::size_t max_nodes;

struct graph {
  using vertex = std::size_t;

  struct resource {
    using pointer = std::unique_ptr<resource>;
    using pack = std::unique_ptr<pointer[]>;

  private:
    friend graph;

    explicit resource(std::size_t size)
        : buffer(std::make_unique<std::byte[]>(size)),
          pool(buffer.get(), size, std::pmr::null_memory_resource()) {}

    static pack alloc(std::size_t vertices, std::size_t edges) {
      pack resources = std::make_unique<pointer[]>(3);
      resources[0] =
          std::make_unique<resource>(vertices * sizeof(graph::vertex));
      resources[1] =
          std::make_unique<resource>(vertices * sizeof(graph::vertex));
      resources[2] = std::make_unique<resource>(edges * sizeof(graph::vertex));
      return resources;
    }

    std::unique_ptr<std::byte[]> buffer;
    std::pmr::monotonic_buffer_resource pool;
  };

  graph(const graph &) = delete;
  graph(graph &&) = default;

  graph &operator=(const graph &) = delete;
  graph &operator=(graph &&) = default;

  static std::pair<graph, resource::pack> build(const char *file);
  static std::pair<graph, resource::pack> build(std::size_t vertices,
                                                std::size_t edges);

private:
  graph(std::size_t V, std::size_t E, resource::pack &pack);

  std::pmr::unordered_map<vertex, std::pmr::unordered_set<vertex>> A;
  std::pmr::vector<std::pmr::vector<bool>> B;
};
} // namespace tsp