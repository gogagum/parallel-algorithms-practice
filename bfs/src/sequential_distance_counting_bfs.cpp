#include <sequential_distance_counting_bfs.hpp>
#include <queue>

void sequential_distance_counting_bfs(ExperimentalGraph &graph) {
  if (graph.nodesCnt() != 0) {
    std::queue<std::size_t> q;
    q.push(0);
    while (!q.empty()) {
      auto currId = q.front();
      q.pop();
      auto currDistance = graph.getNode(currId).getDistanceTo0();

      auto neighbors = graph.getNeighborsRng(currId);
      for (auto nbr : neighbors) {
        if (graph.getNode(nbr).decreaseDistanceTo0(currDistance + 1)) {
          q.push(nbr);
        }
      }
    }
  }
}
