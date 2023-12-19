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

  const auto coords = cartesian_product(sideRng, sideRng, sideRng);

  const auto edgesByCoord = [conv = CubeGraphCoordinatesConverter(sideSize_),
                             sideSize = sideSize_](const auto &coords)
  {
    const auto &[x, y, z] = coords;
    std::vector<std::uint32_t> ret;
    if (x != 0)
    {
      ret.push_back(conv.toId(x - 1, y, z));
    }
    if (y != 0)
    {
      ret.push_back(conv.toId(x, y - 1, z));
    }
    if (z != 0)
    {
      ret.push_back(conv.toId(x, y, z - 1));
    }
    if (x + 1 != sideSize)
    {
      ret.push_back(conv.toId(x + 1, y, z));
    }
    if (y + 1 != sideSize)
    {
      ret.push_back(conv.toId(x, y + 1, z));
    }
    if (z + 1 != sideSize)
    {
      ret.push_back(conv.toId(x, y, z + 1));
    }
    return ret;
  };

  auto nodesDescrs = coords | transform(edgesByCoord);

  return ExperimentalGraph(nodesDescrs);
}
