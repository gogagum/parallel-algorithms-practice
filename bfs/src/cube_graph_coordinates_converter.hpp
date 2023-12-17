#ifndef CUBE_GRAPH_COORDINATES_CONVERTER_HPP
#define CUBE_GRAPH_COORDINATES_CONVERTER_HPP

#include <tuple>

class CubeGraphCoordinatesConverter {
public:
  struct Coords {
    std::size_t xCoord;
    std::size_t yCoord;
    std::size_t zCoord;

    static Coords
    fromTuple(std::tuple<std::size_t, std::size_t, std::size_t> coordsTup) {
      const auto &[x, y, z] = coordsTup;
      return {x, y, z};
    }

    bool operator==(const Coords &other) const = default;
  };

private:
  const std::size_t sideSize_;

  class ToCoords_ {
  private:
    const CubeGraphCoordinatesConverter *owner_;
    ToCoords_(const CubeGraphCoordinatesConverter &owner) : owner_{&owner} {}

  public:
    Coords operator()(std::size_t id) const;

  private:
    friend class CubeGraphCoordinatesConverter;
  };

  class ToId_ {
  private:
    const CubeGraphCoordinatesConverter *owner_;
    ToId_(const CubeGraphCoordinatesConverter &owner) : owner_{&owner} {}

  public:
    std::size_t operator()(std::size_t xCoord, std::size_t yCoord,
                           std::size_t zCoord) const;

    std::size_t operator()(Coords coords) const;

  private:
    friend class CubeGraphCoordinatesConverter;
  };

public:
  CubeGraphCoordinatesConverter(std::size_t sideSize)
      : sideSize_{sideSize}, toCoords{*this}, toId{*this} {};

  const ToCoords_ toCoords;
  const ToId_ toId;
};

////////////////////////////////////////////////////////////////////////////////
inline std::size_t
CubeGraphCoordinatesConverter::ToId_::operator()(Coords coords) const {
  return operator()(coords.xCoord, coords.yCoord, coords.zCoord);
}

#endif
