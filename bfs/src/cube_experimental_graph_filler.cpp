#include "cube_experimental_graph_filler.hpp"
#include "cube_graph_coordinates_converter.hpp"

////////////////////////////////////////////////////////////////////////////////
CubeExperimentalGraphFiller::CubeExperimentalGraphFiller(std::size_t sideSize)
    : sideSize_{sideSize} {}

ExperimentalGraph CubeExperimentalGraphFiller::generate() {
  using std::views::cartesian_product;
  using std::views::drop;
  using std::views::iota;
  using std::views::join;
  using std::views::reverse;
  using std::views::transform;
  using Coords = CubeGraphCoordinatesConverter::Coords;

  auto sideRng = iota(0uz, sideSize_);
  auto directionRng = sideRng | reverse | drop(1);
  auto conv = CubeGraphCoordinatesConverter(sideSize_);

  enum Dir : std::size_t { x = 0, y = 1, z = 2 };
  auto directions = std::array{Dir::x, Dir::y, Dir::z};

  auto edges =
      cartesian_product(sideRng, sideRng, directionRng, directions) |
      transform([&conv](const auto &srcCoords) {
        const auto &[x0, x1, x2, dir] = srcCoords;
        if (dir == Dir::x) {
          return Edge{conv.toId(x2, x0, x1), conv.toId(x2 + 1, x0, x1)};
        }
        if (dir == Dir::y) {
          return Edge{conv.toId(x0, x2, x1), conv.toId(x0, x2 + 1, x1)};
        }
        return Edge{conv.toId(x0, x1, x2), conv.toId(x0, x1, x2 + 1)};
      });

  return ExperimentalGraph(sideSize_ * sideSize_ * sideSize_, edges);
}
