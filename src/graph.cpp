#include "graph.hpp"

#include <stdexcept>

namespace tsp {
const std::size_t max_nodes = 10000;

void graph::add_edge_undirected(vertex u, vertex v, weight w) {
  if (not undirected) {
    throw std::runtime_error(
        "using add_edge_undirected() in a directed graph!");
  }
  add_edge(u, v, w);
  add_edge(v, u, w);
}

void graph::add_edge(vertex u, vertex v, weight w) {
  A[u].insert_or_assign(v, w);
}

bool graph::are_adjacent(vertex u, vertex v) const { return B[u][v]; }

graph::weight graph::get_weight(vertex u, vertex v) const {
  return A.at(u).at(v);
}

const graph::neighbours_map &graph::neighbours(vertex v) const {
  return A.at(v);
}

graph::graph(std::size_t V, bool undirected, resource::pack &pack)
    : undirected(undirected), A(&resource::pool(pack, pool_adj_lst_v)),
      B(V, std::pmr::vector<bool>(V, &resource::pool(pack, pool_adj_mtx)),
        &resource::pool(pack, pool_adj_mtx)) {
  for (std::size_t v = 1; v <= V; ++v) {
    A[v] = neighbours_map(&resource::pool(pack, pool_adj_lst_e));
  }
}

resource::pack graph::alloc(std::size_t vertices, std::size_t edges,
                            bool undirected) {
  resource::pack resources = std::make_unique<resource::pointer[]>(pool_count);
  resources[pool_adj_lst_v] =
      std::make_unique<resource>(vertices * sizeof(adjacency_list::node_type));
  const std::size_t size = not undirected ? edges : 2 * edges;
  resources[pool_adj_lst_e] = std::make_unique<resource>(size);
  resources[pool_adj_mtx] =
      std::make_unique<resource>(vertices * vertices * sizeof(bool));
  return resources;
}

std::pair<graph, resource::pack> graph::build(std::size_t V) {
  return build(V, (V * (V - 1)) / 2);
}

std::pair<graph, resource::pack> graph::build(std::size_t V, std::size_t E,
                                              bool undirected) {
  if (V > max_nodes) {
    throw std::runtime_error("maximum number of vertices exceeded!");
  } else if (E > (V * (V - 1)) / 2) {
    throw std::runtime_error("unreasonable number of edges");
  } else {
    resource::pack resources = alloc(V, E, undirected);
    graph g = graph(V, undirected, resources);
    return std::make_pair(std::move(g), std::move(resources));
  }
}

// std::pair<graph, resource::pack> graph::build(const char *file) { return {};
// }
} // namespace tsp