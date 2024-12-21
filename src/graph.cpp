#include "graph.hpp"

#include <stdexcept>

namespace tsp {
const std::size_t max_nodes = 10000;

graph::graph(std::size_t V, std::size_t E, resource::pack &pack)
    : A(&pack[0]->pool),
      B(V, std::pmr::vector<bool>(V, &pack[1]->pool, &pack[1]->pool)) {
  for (std::size_t v = 1; v <= V; ++v) {
    A[v] = std::pmr::unordered_set<vertex>(&pack[2]->pool);
  }
}

std::pair<graph, graph::resource::pack> graph::build(std::size_t V,
                                                     std::size_t E) {
  return build("");
}

std::pair<graph, graph::resource::pack> graph::build(const char *file) {
  std::size_t V = 1000, E = (V * (V - 1)) / 2;
  if (V > max_nodes) {
    throw std::runtime_error("maximum number of vertices exceeded!");
  }

  resource::pack resources = resource::alloc(V, E);
  graph g = graph(V, E, resources);

  return std::make_pair(std::move(g), std::move(resources));
}
} // namespace tsp