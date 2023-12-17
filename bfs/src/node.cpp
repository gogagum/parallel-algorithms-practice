#include "node.hpp"

////////////////////////////////////////////////////////////////////////////////
void Node::addEdge_(std::size_t id) {
  neighbors_.push_back(id);
}

////////////////////////////////////////////////////////////////////////////////
Node::Node(const Node &other)
    : id_{other.id_}, distanceTo0_{other.distanceTo0_.load()},
      neighbors_(other.neighbors_) {}

////////////////////////////////////////////////////////////////////////////////
bool Node::decreaseDistanceTo0(std::uint64_t distance) {
  std::uint64_t prevDistance = distanceTo0_;
  while (prevDistance > distance &&
         !distanceTo0_.compare_exchange_weak(prevDistance, distance)) {
  }
  return prevDistance > distance;
}

////////////////////////////////////////////////////////////////////////////////
std::span<const std::size_t> Node::getNeighbors() const {
  return {neighbors_.cbegin(), neighbors_.cend()};
}
