#ifndef NODE_HPP
#define NODE_HPP

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <limits>
#include <span>
#include <vector>

namespace detail {
template <bool enableAtomics> struct Distance;

template <> struct Distance<true> {
  using Type = std::atomic_uint64_t;
};

template <> struct Distance<false> {
  using Type = std::uint64_t;
};

template <bool enableAtomics> using DistanceT = Distance<enableAtomics>::Type;
} // namespace detail

template <bool enableAtomics> class Node {
private:
  std::size_t id_;

  detail::DistanceT<enableAtomics> distanceTo0_{
      std::numeric_limits<std::uint64_t>::max()};
  std::vector<std::uint32_t> neighbors_;

private:
  void addEdge_(std::size_t id);

public:
  Node(const Node &other);

  template <std::ranges::input_range IdRng> Node(const IdRng &neighbors);
  bool decreaseDistanceTo0(std::uint64_t distance);
  std::span<const std::uint32_t> getNeighbors() const;
  std::uint64_t getDistanceTo0() const {
    if constexpr (enableAtomics) {
      return distanceTo0_.load();
    } else {
      return distanceTo0_;
    }
  }
  void resetDistanceToInf();

private:
  template <bool enableAtomics_> friend class ExperimentalGraph;
};

////////////////////////////////////////////////////////////////////////////////
template <bool enableAtomics>
template <std::ranges::input_range IdRng>
Node<enableAtomics>::Node(const IdRng &neighbors) : neighbors_{} {
  std::ranges::copy(neighbors, std::back_inserter(neighbors_));
}

////////////////////////////////////////////////////////////////////////////////
template <bool enableAtomics>
void Node<enableAtomics>::addEdge_(std::size_t id) {
  neighbors_.push_back(id);
}

////////////////////////////////////////////////////////////////////////////////
template <bool enableAtomics>
Node<enableAtomics>::Node(const Node<enableAtomics> &other)
    : id_{other.id_}, distanceTo0_{other.getDistanceTo0()},
      neighbors_(other.neighbors_) {}

////////////////////////////////////////////////////////////////////////////////
template <bool enableAtomics>
bool Node<enableAtomics>::decreaseDistanceTo0(std::uint64_t distance) {
  if constexpr (enableAtomics) {
    std::uint64_t prevDistance = distanceTo0_;
    while (prevDistance > distance &&
           !distanceTo0_.compare_exchange_weak(prevDistance, distance)) {
    }
    return prevDistance > distance;
  } else {
    if (distance < distanceTo0_) {
        distanceTo0_ = distance;
        return true;
    }
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////
template <bool enableAtomics>
std::span<const std::uint32_t> Node<enableAtomics>::getNeighbors() const {
  return {neighbors_.cbegin(), neighbors_.cend()};
}

////////////////////////////////////////////////////////////////////////////////
template <bool enableAtomics> void Node<enableAtomics>::resetDistanceToInf() {
  distanceTo0_ = std::numeric_limits<std::uint64_t>::max();
}

#endif
