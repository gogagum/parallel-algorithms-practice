#ifndef EXPERIMENTAL_GRAPH_HPP
#define EXPERIMENTAL_GRAPH_HPP

#include "node.hpp"
#include <algorithm>
#include <span>
#include <vector>

class ExperimentalGraph {
private:
  std::vector<Node> nodes_;

public:
  using NeighborsRng = std::span<const std::uint32_t>;

public:
  template <class NodesDescrRng>
  std::vector<Node> generateNodes_(const NodesDescrRng &nodesDescrRng);

public:
  template <std::ranges::input_range NodesDescrRng>
  ExperimentalGraph(const NodesDescrRng &nodesDescrRng);

  NeighborsRng getNeighborsRng(std::size_t id) const;

  std::size_t nodesCnt() const { return nodes_.size(); }

  Node &getNode(std::size_t id) { return nodes_[id]; };

  void resetDistancesToDefault();
};

template <class NodesDescrRng>
std::vector<Node> generateNodes(const NodesDescrRng &nodesDescrRng) {
  const auto generateNode = [](const auto &neighbors) -> Node {
    return {neighbors};
  };
  std::vector<Node> ret;
  std::ranges::transform(nodesDescrRng, std::back_inserter(ret), generateNode);
  return ret;
}

template <std::ranges::input_range NodesDescrRng>
ExperimentalGraph::ExperimentalGraph(const NodesDescrRng &nodesDescrRng)
    : nodes_(generateNodes(nodesDescrRng)) {
  if (!nodes_.empty()) {
    getNode(0).decreaseDistanceTo0(0);
  }
}

#endif
