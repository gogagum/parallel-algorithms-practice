#include "cube_graph_coordinates_converter.hpp"
#include <gtest/gtest.h>
#include <sequential_distance_counting_bfs.hpp>
#include <cube_experimental_graph_filler.hpp>
#include <array>

TEST(SequentialBfs, OnEmptyGraph) {
    auto graph = CubeExperimentalGraphFiller(0).generate();
    sequential_distance_counting_bfs(graph);
}

TEST(SequentialBfs, OnGraphWithOneNode) {
    auto graph = CubeExperimentalGraphFiller(1).generate();
    sequential_distance_counting_bfs(graph);

    EXPECT_EQ(graph.getNode(0).getDistanceTo0(), 0);
}

TEST(SequentialBfs, OnGraphWithTwoNonConnectedNodes) {
  auto graph = ExperimentalGraph(
      std::array{std::vector<std::size_t>{}, std::vector<std::size_t>{}});
  sequential_distance_counting_bfs(graph);

  EXPECT_EQ(graph.getNode(0).getDistanceTo0(), 0);
  EXPECT_EQ(graph.getNode(1).getDistanceTo0(),
            std::numeric_limits<std::uint64_t>::max());
}

TEST(SequentialBfs, OnGraphWithTwoConnectedNodes) {
    auto graph =
      ExperimentalGraph(std::array{std::vector{1uz}, std::vector{0uz}});
    sequential_distance_counting_bfs(graph);

    EXPECT_EQ(graph.getNode(0).getDistanceTo0(), 0);
    EXPECT_EQ(graph.getNode(1).getDistanceTo0(), 1);
}

TEST(SequentialBfs, BambooThree) {
    auto graph = ExperimentalGraph(std::array{
      std::vector{1uz}, std::vector{0uz, 2uz}, std::vector{1uz}});
    sequential_distance_counting_bfs(graph);

    EXPECT_EQ(graph.getNode(0).getDistanceTo0(), 0);
    EXPECT_EQ(graph.getNode(1).getDistanceTo0(), 1);
    EXPECT_EQ(graph.getNode(2).getDistanceTo0(), 2);
}

TEST(SequentialBfs, CubeFive) {
    auto graph = CubeExperimentalGraphFiller(5).generate();
    sequential_distance_counting_bfs(graph);
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