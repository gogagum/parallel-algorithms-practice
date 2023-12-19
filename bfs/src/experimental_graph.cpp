#include "experimental_graph.hpp"
#include <tbb/parallel_for.h>

////////////////////////////////////////////////////////////////////////////////
auto ExperimentalGraph::getNeighborsRng(std::size_t id) const -> NeighborsRng {
  return nodes_[id].getNeighbors();
}

////////////////////////////////////////////////////////////////////////////////
void ExperimentalGraph::resetDistancesToDefault() {
  if (0 != nodesCnt()) {
    tbb::parallel_for(1uz, nodes_.size(), [this](std::size_t i)
                      { nodes_[i].resetDistanceToInf(); });
  }
}