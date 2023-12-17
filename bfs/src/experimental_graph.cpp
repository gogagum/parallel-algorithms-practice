#include "experimental_graph.hpp"

////////////////////////////////////////////////////////////////////////////////
auto ExperimentalGraph::getNeighborsRng(std::size_t id) const -> NeighborsRng {
  return nodes_[id].getNeighbors();
}
