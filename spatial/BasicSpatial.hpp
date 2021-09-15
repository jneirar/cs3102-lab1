#pragma once

#include "SpatialBase.h"
#include <limits>

namespace utec {
namespace spatial {

/**
 * BasicSpatial implementation
 */
template <typename Point>
class BasicSpatial : public SpatialBase<Point> {
 private:
 std::vector<Point> points;

 public:
  BasicSpatial() {};
  void insert(const Point& new_point) override {
    points.push_back(new_point);
  }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override {
    double distance = std::numeric_limits<double>::max();
    Point p;

    for(auto& point : points){
      double new_distance = reference.distance(point);
      if(new_distance < distance){
        distance = new_distance;
        p = point;
      }
    }

    return p;
  }
};

}  // namespace spatial
}  // namespace utec
