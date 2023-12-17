#include "cube_graph_coordinates_converter.hpp"

////////////////////////////////////////////////////////////////////////////////
auto CubeGraphCoordinatesConverter::ToCoords_::operator()(std::size_t id) const
    -> Coords {
  auto xCoord = id / (owner_->sideSize_ * owner_->sideSize_);
  auto yCoord = (id / owner_->sideSize_) % owner_->sideSize_;
  auto zCoord = id % owner_->sideSize_;
  return {xCoord, yCoord, zCoord};
}

////////////////////////////////////////////////////////////////////////////////
std::size_t CubeGraphCoordinatesConverter::ToId_::operator()(
    std::size_t xCoord, std::size_t yCoord, std::size_t zCoord) const {
  return xCoord * owner_->sideSize_ * owner_->sideSize_ +
         yCoord * owner_->sideSize_ + zCoord;
}
