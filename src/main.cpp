#include "graph.hpp"

int main() {
  auto [g, resources] = tsp::graph::build("tsp/ja9847.tsp");
  return 0;
}