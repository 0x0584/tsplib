#include "graph.hpp"

#include <stdexcept>

namespace tsp {
const std::size_t max_nodes = 10000;

graph::graph(std::size_t V, std::size_t E, resource::pack &pack)
    : A(&pack[0]->get()),
      B(V, std::pmr::vector<bool>(V, &pack[1]->get(), &pack[1]->get())) {
  for (std::size_t v = 1; v <= V; ++v) {
    A[v] = std::pmr::unordered_set<vertex>(&pack[2]->get());
  }
}

resource::pack graph::alloc(std::size_t vertices, std::size_t edges) {
  resource::pack resources = std::make_unique<resource::pointer[]>(3);
  resources[0] = std::make_unique<resource>(vertices * sizeof(vertex));
  resources[1] = std::make_unique<resource>(vertices * sizeof(vertex));
  resources[2] = std::make_unique<resource>(edges * sizeof(vertex));
  return resources;
}

std::pair<graph, resource::pack> graph::build(std::size_t V, std::size_t E) {
  if (V > max_nodes) {
    throw std::runtime_error("maximum number of vertices exceeded!");
  } else if (E > (V * (V - 1)) / 2) {
    throw std::runtime_error("unreasonable number of edges");
  }

  resource::pack resources = alloc(V, E);
  graph g = graph(V, E, resources);

  return std::make_pair(std::move(g), std::move(resources));
}

// std::pair<graph, resource::pack> graph::build(const char *file) { return {};
// }
} // namespace tsp