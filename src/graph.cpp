#include "graph.hpp"

#include <stdexcept>

namespace tsp {
const std::size_t max_nodes = 10000;

void graph::add_edge_undirected(vertex u, vertex v, weight w) {
  add_edge(u, v, w);
  add_edge(v, u, w);
}

void graph::add_edge(vertex u, vertex v, weight w) {
  A[u].emplace(v).second;
  W[{u, v}] = w;
}

bool graph::are_adjacent(vertex u, vertex v) const { return B[u][v]; }
graph::weight graph::get_weight(vertex u, vertex v) const {
  return W.at({u, v});
}

const std::pmr::unordered_set<graph::vertex> &
graph::neighbours(vertex v) const {
  return A.at(v);
}

graph::graph(std::size_t V, std::size_t E, resource::pack &pack)
    : A(resource::resolve(pack, pool_adj_lst_v)),
      W(resource::resolve(pack, pool_weights)),
      B(V, std::pmr::vector<bool>(V, resource::resolve(pack, pool_adj_mtx)),
        resource::resolve(pack, pool_adj_mtx)) {
  for (std::size_t v = 1; v <= V; ++v) {
    A[v] = std::pmr::unordered_set<vertex>(&pack[pool_adj_lst_e]->get());
  }
}

resource::pack graph::alloc(std::size_t vertices, std::size_t edges) {
  resource::pack resources = std::make_unique<resource::pointer[]>(pool_count);
  resources[pool_adj_lst_v] = std::make_unique<resource>(vertices * sizeof(vertex));
  resources[pool_adj_lst_e] = std::make_unique<resource>(vertices * sizeof(vertex));
  resources[pool_weights] = std::make_unique<resource>(edges * sizeof(vertex));
  resources[pool_adj_mtx] = std::make_unique<resource>(edges * sizeof(vertex));
  return resources;
}

std::pair<graph, resource::pack> graph::build(std::size_t V) {
  return build(V, (V * (V - 1)) / 2);
}

std::pair<graph, resource::pack> graph::build(std::size_t V, std::size_t E) {
  if (V > max_nodes) {
    throw std::runtime_error("maximum number of vertices exceeded!");
  } else if (E > (V * (V - 1)) / 2) {
    throw std::runtime_error("unreasonable number of edges");
  } else {
    resource::pack resources = alloc(V, E);
    graph g = graph(V, E, resources);
    return std::make_pair(std::move(g), std::move(resources));
  }
}

// std::pair<graph, resource::pack> graph::build(const char *file) { return {};
// }
} // namespace tsp