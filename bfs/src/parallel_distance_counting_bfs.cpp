#include <parallel_distance_counting_bfs.hpp>

namespace detail {
void process_layer(ExperimentalGraph &graph,
                   tbb::concurrent_vector<std::size_t> &srcVec,
                   tbb::concurrent_vector<std::size_t> &destVec) {
  tbb::parallel_for(
      0uz, srcVec.size(), [&graph, &srcVec, &destVec](std::size_t i) {
        const auto id = srcVec[i];
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
} // namespace detail

void parallel_distance_counting_bfs(ExperimentalGraph &graph) {
  detail::parallel_distance_counting_bfs_impl<false>(graph);
}
