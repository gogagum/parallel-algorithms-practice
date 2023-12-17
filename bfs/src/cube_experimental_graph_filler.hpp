#ifndef CUBE_EXPERIMENTAL_GRAPH_FILLER_HPP
#define CUBE_EXPERIMENTAL_GRAPH_FILLER_HPP

#include "experimental_graph.hpp"

class CubeExperimentalGraphFiller {
private:
  std::size_t sideSize_;

public:
  CubeExperimentalGraphFiller(std::size_t sideSize);

  ExperimentalGraph generate();
};

#endif
