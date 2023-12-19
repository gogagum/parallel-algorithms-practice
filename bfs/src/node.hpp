#ifndef NODE_HPP
#define NODE_HPP

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <limits>
#include <span>
#include <vector>

class Node {
private:
  std::size_t id_;
  std::atomic_uint64_t distanceTo0_{std::numeric_limits<std::uint64_t>::max()};
  // TODO(gogagum): make possible to disable atomic
  std::vector<std::uint32_t> neighbors_;

private:
  void addEdge_(std::size_t id);

public:
  Node(const Node &other);

  template <std::ranges::input_range IdRng>
  Node(const IdRng &neighbors);
  bool decreaseDistanceTo0(std::uint64_t distance);
  std::span<const std::uint32_t> getNeighbors() const;
  std::uint64_t getDistanceTo0() const { return distanceTo0_.load(); }
  void resetDistanceToInf();

private:
  friend class ExperimentalGraph;
};

////////////////////////////////////////////////////////////////////////////////
template <std::ranges::input_range IdRng>
Node::Node(const IdRng &neighbors) : neighbors_{} {
  std::ranges::copy(neighbors, std::back_inserter(neighbors_));
}

#endif
