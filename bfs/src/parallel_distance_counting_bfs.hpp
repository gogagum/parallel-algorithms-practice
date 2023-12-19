#ifndef PARALLEL_DISTANCE_COUNTING_BFS_HPP
#define PARALLEL_DISTANCE_COUNTING_BFS_HPP

#include "experimental_graph.hpp"
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>

namespace detail {

void process_layer(ExperimentalGraph &graph,
                   tbb::concurrent_vector<std::size_t> &srcVec,
                   tbb::concurrent_vector<std::size_t> &destVec);

template <std::size_t blockSize>
void process_layer_blocked(ExperimentalGraph &graph,
                           tbb::concurrent_vector<std::size_t> &srcVec,
                           tbb::concurrent_vector<std::size_t> &destVec) {
  auto blocksCnt = (srcVec.size() - 1) / blockSize + 1;
  tbb::parallel_for(
      0uz, blocksCnt, [&graph, &destVec, &srcVec](std::size_t blockI) {
        std::vector<std::size_t> toVisit;
        auto beginI = blockI * blockSize;
        auto endI = std::min(beginI + blockSize, srcVec.size());
        for (auto i : std::views::iota(beginI, endI)) {
          const auto id = srcVec[i];
          auto currDistance = graph.getNode(id).getDistanceTo0();
          auto neighbors = graph.getNeighborsRng(id);
          for (auto nbr : neighbors) {
            if (graph.getNode(nbr).decreaseDistanceTo0(currDistance + 1)) {
              toVisit.push_back(nbr);
            }
          }
        }
        destVec.grow_by(toVisit.begin(), toVisit.end());
      });
}

template <auto blocked, auto... args>
void parallel_distance_counting_bfs_impl(ExperimentalGraph &graph) {
  if (graph.nodesCnt() != 0) {
    tbb::concurrent_vector<std::size_t> q1;
    tbb::concurrent_vector<std::size_t> q2;

    using QueueRef =
        std::reference_wrapper<tbb::concurrent_vector<std::size_t>>;
    using QueueRefPair = std::pair<QueueRef, QueueRef>;

    q1.push_back(0);
    const auto getQueuesPair = [&q1, &q2](std::size_t layerIndex) {
      return (layerIndex % 2 == 0) ? QueueRefPair{q1, q2}
                                   : QueueRefPair{q2, q1};
    };

    for (std::size_t layerIndex = 0;
         !getQueuesPair(layerIndex).first.get().empty(); ++layerIndex) {
      auto [q1, q2] = getQueuesPair(layerIndex);
      q2.get().clear();
      if constexpr (blocked) {
        process_layer_blocked<args...>(graph, q1, q2);
      } else {
        static_assert(sizeof...(args) == 0,
                      "No args needed for non-blocked algorithm.");
        process_layer(graph, q1, q2);
      }
    }
  }
}

} // namespace detail

void parallel_distance_counting_bfs(ExperimentalGraph &graph);

template <std::size_t blockSize>
void parallel_distance_counting_bfs_blocked(ExperimentalGraph &graph) {
  detail::parallel_distance_counting_bfs_impl<true, blockSize>(graph);
}

#endif
