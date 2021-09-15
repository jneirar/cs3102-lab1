#pragma once

#include "SpatialBase.h"
#include <limits>
#include <list>

namespace utec {
namespace spatial {

/**
 * BasicGridSpatial implementation
 */
template <typename Point>
class BasicGridSpatial : public SpatialBase<Point> {
 private:
  using bucket_idx_t = std::size_t;

  std::size_t _MAX;
  std::size_t _m;

  //Estructura de Datos Principal
  std::vector<std::list<Point>> bucket_list;

  // TODO: Una funcion que en base a un bucket, te de los vecinos 
  std::vector<bucket_idx_t>  get_bucket_neighbours(bucket_idx_t src);

  // Mapea un punto a su correspondiente índice de bucket
  bucket_idx_t get_bucket(const Point& new_point){
    auto y = new_point.get(1);
    auto x = new_point.get(0);

    return floor(y/(_MAX/(double)_m))*_m +floor(x/(_MAX/(double)_m));
  }

 public:
  BasicGridSpatial(std::size_t MAX, std::size_t m) : _MAX(MAX), _m(m) {
    bucket_list.resize(_m*_m);
  };

  void insert(const Point& new_point) override {
    auto n_bucket = get_bucket(new_point);
    bucket_list[n_bucket].push_back(new_point);
  }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override {
    auto n_bucket = get_bucket(reference);
    distance_t  min = 1e9;
    Point answer;
    for (const auto &p : bucket_list[n_bucket]) {
      if (reference.distance(p)< min){
        min = reference.distance(p);
        answer=p;
      }
    }
    return answer;
  }
};

}  // namespace spatial
}  // namespace utec
