#include <array>
#include <cube_experimental_graph_filler.hpp>
#include <cube_graph_coordinates_converter.hpp>
#include <experimental_graph.hpp>
#include <gtest/gtest.h>

TEST(CubeGraphCoordinatesConverter, ToCoords) {
  auto conv = CubeGraphCoordinatesConverter(3);
  using Coords = CubeGraphCoordinatesConverter::Coords;

  {
    auto coords = Coords{0, 0, 0};
    EXPECT_EQ(conv.toCoords(0), coords);
  }
  {
    auto coords = Coords{0, 0, 2};
    EXPECT_EQ(conv.toCoords(2), coords);
  }
  {
    auto coords = Coords{0, 2, 0};
    EXPECT_EQ(conv.toCoords(6), coords);
  }
  {
    auto coords = Coords{2, 0, 0};
    EXPECT_EQ(conv.toCoords(18), coords);
  }
  {
    auto coords = Coords{0, 2, 2};
    EXPECT_EQ(conv.toCoords(8), coords);
  }
  {
    auto coords = Coords{2, 2, 0};
    EXPECT_EQ(conv.toCoords(24), coords);
  }
  {
    auto coords = Coords{2, 0, 2};
    EXPECT_EQ(conv.toCoords(20), coords);
  }
  {
    auto coords = Coords{2, 2, 2};
    EXPECT_EQ(conv.toCoords(26), coords);
  }
  {
    auto сoords = Coords{1, 1, 1};
    EXPECT_EQ(conv.toCoords(13uz), сoords);
  }
}

TEST(CubeGraphCoordinatesConverter, ToId) {
  auto conv = CubeGraphCoordinatesConverter(3);

  EXPECT_EQ(conv.toId(0, 0, 0), 0);
  EXPECT_EQ(conv.toId(0, 0, 2), 2);
  EXPECT_EQ(conv.toId(0, 2, 0), 6);
  EXPECT_EQ(conv.toId(2, 0, 0), 18);
  EXPECT_EQ(conv.toId(0, 2, 2), 8);
  EXPECT_EQ(conv.toId(2, 2, 0), 24);
  EXPECT_EQ(conv.toId(2, 0, 2), 20);
  EXPECT_EQ(conv.toId(2, 2, 2), 26);
  EXPECT_EQ(conv.toId(1, 1, 1), 13);
}

TEST(GraphForDistanceCounting, Construct) {
  auto graph = ExperimentalGraph(std::array<std::vector<std::size_t>, 0>{});
  EXPECT_TRUE(graph.getNeighborsRng(0).empty());
}

TEST(GraphForDistanceCounting, TwoNonConnectedNodes) {
  auto graph = ExperimentalGraph(std::array<std::vector<std::size_t>, 2>{
      std::vector<std::size_t>{}, std::vector<std::size_t>{}});
  EXPECT_TRUE(graph.getNeighborsRng(0).empty());
  EXPECT_TRUE(graph.getNeighborsRng(1).empty());
}

TEST(GraphForDistanceCounting, TwoConnectedNodes) {
  auto graph = ExperimentalGraph(std::array<std::vector<std::size_t>, 2>{
      std::vector{1uz}, std::vector{0uz}});
  EXPECT_EQ(graph.getNeighborsRng(0).size(), 1);
  EXPECT_EQ(graph.getNeighborsRng(0)[0], 1);
  EXPECT_EQ(graph.getNeighborsRng(1).size(), 1);
  EXPECT_EQ(graph.getNeighborsRng(1)[0], 0);
}

TEST(GraphFilling, One) {
  auto graph = CubeExperimentalGraphFiller(1).generate();
  EXPECT_EQ(graph.nodesCnt(), 1);
  EXPECT_TRUE(graph.getNeighborsRng(0).empty());
}

TEST(GraphFilling, Two) {
  auto graph = CubeExperimentalGraphFiller(2).generate();
  EXPECT_EQ(graph.nodesCnt(), 8);
  EXPECT_EQ(graph.getNeighborsRng(0).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(1).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(2).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(3).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(4).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(5).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(6).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(7).size(), 3);

  // 0 1
  // 2 3

  // 4 5
  // 6 7

  {
    auto expectedZeroNeighbors = std::set{1, 2, 4};
    auto actualZeroNeighbors = graph.getNeighborsRng(0);
    auto actualZeroNeighborsSet =
        std::set(actualZeroNeighbors.begin(), actualZeroNeighbors.end());

    EXPECT_TRUE(
        std::ranges::equal(expectedZeroNeighbors, actualZeroNeighborsSet));
  }

  {
    auto expectedFiveNeighbors = std::set{1, 4, 7};

    auto actualFiveNeighbors = graph.getNeighborsRng(5);
    auto actualFiveNeighborsSet =
        std::set(actualFiveNeighbors.begin(), actualFiveNeighbors.end());

    EXPECT_TRUE(
        std::ranges::equal(expectedFiveNeighbors, actualFiveNeighborsSet));
  }
}

TEST(GraphFilling, Five) {
  auto graph = CubeExperimentalGraphFiller(5).generate();
  EXPECT_EQ(graph.nodesCnt(), 125);
  EXPECT_EQ(graph.getNeighborsRng(0).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(4).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(20).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(24).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(120).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(100).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(104).size(), 3);
  EXPECT_EQ(graph.getNeighborsRng(124).size(), 3);

  EXPECT_EQ(graph.getNeighborsRng(3).size(), 4);
  EXPECT_EQ(graph.getNeighborsRng(15).size(), 4);
  EXPECT_EQ(graph.getNeighborsRng(14).size(), 4);
  EXPECT_EQ(graph.getNeighborsRng(21).size(), 4);
  EXPECT_EQ(graph.getNeighborsRng(50).size(), 4);
  EXPECT_EQ(graph.getNeighborsRng(54).size(), 4);

  // 0   1   ... 4
  // 5   6   ... 9
  // ... ... ... ...
  // 20  21  ... 24

  // 100 101 ... 104
  // 105 106 ... 109
  // ... ... ... ...
  // 120 121 ... 124

  {
    auto expectedZeroNeighbors = std::set{1, 5, 25};
    auto actualZeroNeighbors = graph.getNeighborsRng(0);
    auto actualZeroNeighborsSet =
        std::set(actualZeroNeighbors.begin(), actualZeroNeighbors.end());

    EXPECT_TRUE(
        std::ranges::equal(expectedZeroNeighbors, actualZeroNeighborsSet));
  }

  {
    const auto conv = CubeGraphCoordinatesConverter(5);
    auto [x82, y82, z82] = conv.toCoords(82);
    auto xNbr0 = conv.toId(x82 - 1, y82, z82);
    auto xNbr1 = conv.toId(x82 + 1, y82, z82);
    auto yNbr0 = conv.toId(x82, y82 - 1, z82);
    auto yNbr1 = conv.toId(x82, y82 + 1, z82);
    auto zNbr0 = conv.toId(x82, y82, z82 - 1);
    auto zNbr1 = conv.toId(x82, y82, z82 + 1);

    auto expected82Neighbors =
        std::set{xNbr0, xNbr1, yNbr0, yNbr1, zNbr0, zNbr1};

    auto actual82Neighbors = graph.getNeighborsRng(82);
    auto actual82NeighborsSet =
        std::set(actual82Neighbors.begin(), actual82Neighbors.end());

    EXPECT_TRUE(std::ranges::equal(expected82Neighbors, actual82NeighborsSet));
  }
}
