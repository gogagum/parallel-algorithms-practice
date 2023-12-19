#ifndef EXPERIMENTAL_GRAPH_HPP
#define EXPERIMENTAL_GRAPH_HPP

#include "edge.hpp"
#include "node.hpp"
#include <algorithm>
#include <ranges>
#include <span>
#include <type_traits>
#include <vector>

class ExperimentalGraph {
private:
  std::vector<Node> nodes_;

public:
  using NeighborsRng = std::span<const std::size_t>;

public:
  template <class EdgesRng>
  std::vector<Node> generateNodes_(std::size_t nodesCnt,
                                   const EdgesRng &vertices);

public:
  template <std::ranges::input_range EdgesRng>
    requires std::is_same_v<std::ranges::range_value_t<EdgesRng>, Edge>
  ExperimentalGraph(std::size_t nodesCnt, const EdgesRng &vertices);

  NeighborsRng getNeighborsRng(std::size_t id) const;

  std::size_t nodesCnt() const { return nodes_.size(); }

  Node &getNode(std::size_t id) { return nodes_[id]; };

  void resetDistancesToDefault();
};

template <class EdgesRng>
std::vector<Node> generateNodes(std::size_t nodesCnt, const EdgesRng &edges) {
  std::vector<std::vector<std::size_t>> edgesProcessed(nodesCnt);
  std::ranges::for_each(edges, [&edgesProcessed](const auto &edge) {
    edgesProcessed[edge.id0].push_back(edge.id1);
    edgesProcessed[edge.id1].push_back(edge.id0);
  });

  const auto generateNode = [&edgesProcessed](std::size_t id) -> Node {
    return {id, edgesProcessed[id]};
  };
  std::vector<Node> ret;
  std::ranges::transform(std::views::iota(0uz, nodesCnt),
                         std::back_inserter(ret), generateNode);
  return ret;
}

template <std::ranges::input_range EdgesRng>
  requires std::is_same_v<std::ranges::range_value_t<EdgesRng>, Edge>
ExperimentalGraph::ExperimentalGraph(std::size_t nodesCnt,
                                     const EdgesRng &edges)
    : nodes_(generateNodes(nodesCnt, edges)) {
  if (!nodes_.empty()) {
    getNode(0).decreaseDistanceTo0(0);
  }
}

#endif
