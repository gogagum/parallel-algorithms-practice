#include "cube_graph_coordinates_converter.hpp"
#include <array>
#include <cube_experimental_graph_filler.hpp>
#include <gtest/gtest.h>
#include <parallel_distance_counting_bfs.hpp>

TEST(ParallelBfs, OnEmptyGraph) {
  auto graph = CubeExperimentalGraphFiller(0).generate();
  parallel_distance_counting_bfs(graph);
}

TEST(ParallelBfs, OnGraphWithOneNode) {
  auto graph = CubeExperimentalGraphFiller(1).generate();
  parallel_distance_counting_bfs(graph);

  EXPECT_EQ(graph.getNode(0).getDistanceTo0(), 0);
}

TEST(ParallelBfs, OnGraphWithTwoNonConnectedNodes) {
  auto graph = ExperimentalGraph(
      std::array{std::vector<std::size_t>{}, std::vector<std::size_t>{}});
  parallel_distance_counting_bfs(graph);

  EXPECT_EQ(graph.getNode(0).getDistanceTo0(), 0);
  EXPECT_EQ(graph.getNode(1).getDistanceTo0(),
            std::numeric_limits<std::uint64_t>::max());
}

TEST(ParallelBfs, OnGraphWithTwoConnectedNodes) {
  auto graph =
      ExperimentalGraph(std::array{std::vector{1uz}, std::vector{0uz}});
  parallel_distance_counting_bfs(graph);

  EXPECT_EQ(graph.getNode(0).getDistanceTo0(), 0);
  EXPECT_EQ(graph.getNode(1).getDistanceTo0(), 1);
}

TEST(ParallelBfs, BambooThree) {
  auto graph = ExperimentalGraph(
      std::array{std::vector{1uz}, std::vector{0uz, 2uz}, std::vector{1uz}});
  parallel_distance_counting_bfs(graph);

  EXPECT_EQ(graph.getNode(0).getDistanceTo0(), 0);
  EXPECT_EQ(graph.getNode(1).getDistanceTo0(), 1);
  EXPECT_EQ(graph.getNode(2).getDistanceTo0(), 2);
}

TEST(ParallelBfs, CubeFive) {
  auto graph = CubeExperimentalGraphFiller(5).generate();
  parallel_distance_counting_bfs(graph);
  auto conv = CubeGraphCoordinatesConverter(5);

  {
    auto id = conv.toId(2, 3, 4);
    EXPECT_EQ(graph.getNode(id).getDistanceTo0(), 2 + 3 + 4);
  }

  {
    auto id = conv.toId(2, 2, 2);
    EXPECT_EQ(graph.getNode(id).getDistanceTo0(), 2 + 2 + 2);
  }

  {
    auto id = conv.toId(2, 0, 1);
    EXPECT_EQ(graph.getNode(id).getDistanceTo0(), 2 + 1);
  }
}

TEST(ParallelBfs, Cube100) {
  auto graph = CubeExperimentalGraphFiller(100).generate();
  parallel_distance_counting_bfs(graph);
  auto conv = CubeGraphCoordinatesConverter(100);

  {
    auto id = conv.toId(51, 52, 53);
    EXPECT_EQ(graph.getNode(id).getDistanceTo0(), 51 + 52 + 53);
  }

  {
    auto id = conv.toId(98, 97, 34);
    EXPECT_EQ(graph.getNode(id).getDistanceTo0(), 98 + 97 + 34);
  }

  {
    auto id = conv.toId(99, 99, 99);
    EXPECT_EQ(graph.getNode(id).getDistanceTo0(), 99 * 3);
  }
}