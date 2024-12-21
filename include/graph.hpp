#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "resource.hpp"

namespace tsp {
extern const std::size_t max_nodes;

struct graph {
  using vertex = std::size_t;
  using weight = long double;

  using neighbours = std::pmr::unordered_set<vertex>;
  using adjacency_list = std::pmr::unordered_map<vertex, neighbours>;
  using weight_list =
      std::pmr::unordered_map<std::pair<vertex, vertex>, weight>;
  using adjacency_matrix = std::pmr::vector<std::pmr::vector<bool>>;

  graph(const graph &) = delete;
  graph(graph &&) = default;

  graph &operator=(const graph &) = delete;
  graph &operator=(graph &&) = default;

  void add_edge_undirected(vertex u, vertex v, weight w);
  void add_edge(vertex u, vertex v, weight w);
  bool are_adjacent(vertex u, vertex v) const;
  weight get_weight(vertex u, vertex v) const;
  const neighbours &neighbours(vertex v) const;

  // static std::pair<graph, resource::pack> build(const char *file);
  static std::pair<graph, resource::pack> build(std::size_t vertices);
  static std::pair<graph, resource::pack> build(std::size_t vertices,
                                                std::size_t edges);

private:
  enum resource_pools {
    pool_adj_lst_v = 0,
    pool_adj_lst_e,
    pool_adj_mtx,
    pool_weights,
    pool_count
  };

  static resource::pack alloc(std::size_t vertices, std::size_t edges);
  graph(std::size_t V, std::size_t E, resource::pack &pack);

  adjacency_list A;
  weight_list W;
  adjacency_matrix B;
};
} // namespace tsp