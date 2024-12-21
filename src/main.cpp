#include "graph.hpp"

int main() {
  auto [g, resources] = tsp::graph::build(33, 100);
  return 0;
}