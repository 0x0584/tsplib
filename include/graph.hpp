#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "resource.hpp"

namespace tsp {
extern const std::size_t max_nodes;

struct graph {
  using vertex = std::size_t;

  graph(const graph &) = delete;
  graph(graph &&) = default;

  graph &operator=(const graph &) = delete;
  graph &operator=(graph &&) = default;

  // static std::pair<graph, resource::pack> build(const char *file);
  static std::pair<graph, resource::pack> build(std::size_t vertices,
                                                std::size_t edges);

private:
  static resource::pack alloc(std::size_t vertices, std::size_t edges);
  graph(std::size_t V, std::size_t E, resource::pack &pack);

  std::pmr::unordered_map<vertex, std::pmr::unordered_set<vertex>> A;
  std::pmr::vector<std::pmr::vector<bool>> B;
};
} // namespace tsp