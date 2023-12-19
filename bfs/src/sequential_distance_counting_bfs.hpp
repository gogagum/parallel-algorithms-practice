#ifndef SEQUENTIAL_DISTANCE_COUNTING_BFS
#define SEQUENTIAL_DISTANCE_COUNTING_BFS

#include "experimental_graph.hpp"
#include <queue>

template <bool enableAtomics>
void sequential_distance_counting_bfs(ExperimentalGraph<enableAtomics> &graph){
  if (graph.nodesCnt() != 0) {
    std::queue<std::size_t> q;
    q.push(0);
    while (!q.empty()) {
      auto currId = q.front();
      q.pop();
      auto currDistance = graph.getNode(currId).getDistanceTo0();

      auto neighbors = graph.getNode(currId).getNeighbors();
      for (auto nbr : neighbors) {
        if (graph.getNode(nbr).decreaseDistanceTo0(currDistance + 1)) {
          q.push(nbr);
        }
      }
    }
  }
}

#endif  // SEQUENTIAL_DISTANCE_COUNTING_BFS
