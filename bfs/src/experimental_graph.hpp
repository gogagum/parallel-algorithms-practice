#ifndef EXPERIMENTAL_GRAPH_HPP
#define EXPERIMENTAL_GRAPH_HPP

#include "node.hpp"
#include <algorithm>
#include <span>
#include <vector>
#include <tbb/parallel_for.h>

template <bool enableAtomics = true>
class ExperimentalGraph {
private:
  using Node_ = Node<enableAtomics>;
  std::vector<Node_> nodes_;

public:
  using NeighborsRng = std::span<const std::uint32_t>;

private:
  template <class NodesDescrRng>
  static std::vector<Node_> generateNodes_(const NodesDescrRng &nodesDescrRng);

public:
  template <std::ranges::input_range NodesDescrRng>
  ExperimentalGraph(const NodesDescrRng &nodesDescrRng);

  /**
   * @brief Get nodes count.
   * 
   * @return std::size_t 
   */
  std::size_t nodesCnt() const { return nodes_.size(); }

  /**
   * @brief Get node reference.
   * 
   * @param id 
   * @return Node_& 
   */
  Node_ &getNode(std::size_t id) { return nodes_[id]; };

  /**
   * @brief Get Node reference.
   * 
   * @param id 
   * @return const Node& 
   */
  const Node_& getNode(std::size_t id) const { return nodes_[id]; };

  /**
   * @brief Reset distance for all nodes excluding #0.
   */
  void resetDistancesToDefault();
};

template <bool enableAtomics>
template <class NodesDescrRng>
auto ExperimentalGraph<enableAtomics>::generateNodes_(
    const NodesDescrRng &nodesDescrRng) -> std::vector<Node_> {
  const auto generateNode = [](const auto &neighbors) -> Node_ {
    return {neighbors};
  };
  std::vector<Node_> ret;
  std::ranges::transform(nodesDescrRng, std::back_inserter(ret), generateNode);
  return ret;
}

template <bool enableAtomics>
template <std::ranges::input_range NodesDescrRng>
ExperimentalGraph<enableAtomics>::ExperimentalGraph(
    const NodesDescrRng &nodesDescrRng)
    : nodes_(generateNodes_(nodesDescrRng)) {
  if (!nodes_.empty()) {
    getNode(0).decreaseDistanceTo0(0);
  }
}

////////////////////////////////////////////////////////////////////////////////
template <bool enableAtomics>
void ExperimentalGraph<enableAtomics>::resetDistancesToDefault() {
  if (0 != nodesCnt()) {
    tbb::parallel_for(1uz, nodes_.size(), [this](std::size_t i) {
      nodes_[i].resetDistanceToInf();
    });
  }
}

#endif
