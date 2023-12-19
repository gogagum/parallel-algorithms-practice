#include <algorithm>
#include <execution>
#include <parallel_distance_counting_bfs.hpp>
#include <tbb/concurrent_vector.h>

namespace {

void process_layer(ExperimentalGraph &graph,
                   tbb::concurrent_vector<std::size_t> &srcVec,
                   tbb::concurrent_vector<std::size_t> &destVec) {
  std::for_each(
      std::execution::par_unseq, srcVec.begin(), srcVec.end(),
      [&graph, &destVec](std::size_t id) {
        auto currDistance = graph.getNode(id).getDistanceTo0();
        auto neighbors = graph.getNeighborsRng(id);
        std::vector<std::size_t> toVisit;
        for (auto nbr : neighbors) {
          if (graph.getNode(nbr).decreaseDistanceTo0(currDistance + 1)) {
            toVisit.push_back(nbr);
          }
        }
        destVec.grow_by(toVisit.begin(), toVisit.end());
      });
}
} // namespace

void parallel_distance_counting_bfs(ExperimentalGraph &graph) {
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
      process_layer(graph, q1, q2);
    }
  }
}
