#pragma once

#include <limits>
#include <list>

#include "SpatialBase.h"

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

  // Estructura de Datos Principal
  std::vector<std::list<Point>> bucket_list;

 public:
  // TODO: Una funcion que en base a un bucket, te de los vecinos
  std::vector<bucket_idx_t> get_bucket_neighbours(bucket_idx_t src) {
    // auto candidates = {src + 1, src - 1, src + m, src - m, src + m + 1, src +
    // m - 1, src - m + 1, src - m - 1};
    // for(auto& c : candidates){
    // }

    std::vector<int> int_array{-1, 0, 1};
    std::vector<int> ext_array{static_cast<int>(-_m), 0, static_cast<int>(_m)};

    std::vector<bucket_idx_t> result;
    for (int exterior : ext_array) {
      for (int interior : int_array) {
        int candidate = src + exterior + interior;

        if (src < _m) {  // bottom borderline
          if (exterior == -_m) continue;
        }
        if (src % _m == 0) {  // left borderline
          if (interior == -1) continue;
        }
        if ((src + 1) % _m == 0) {  // right borderline
          if (interior == 1) continue;
        }
        if (src >= (_m * (_m - 1))) {  // top borderline
          if (exterior == _m) continue;
        }
        result.push_back(candidate);
      }
    }

    return result;
  }

  // Mapea un punto a su correspondiente índice de bucket
  bucket_idx_t get_bucket(const Point& new_point) {
    auto y = new_point.get(1);
    auto x = new_point.get(0);

    return floor(y / (_MAX / (double)_m)) * _m + floor(x / (_MAX / (double)_m));
  }

 public:
  BasicGridSpatial(std::size_t MAX, std::size_t m) : _MAX(MAX), _m(m) {
    bucket_list.resize(_m * _m);
  };

  void insert(const Point& new_point) override {
    auto n_bucket = get_bucket(new_point);
    bucket_list[n_bucket].push_back(new_point);
  }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override {
    auto n_bucket = get_bucket(reference);
    distance_t min = std::numeric_limits<distance_t>::max();
    Point answer;
    for (const auto& bucket_idx : get_bucket_neighbours(n_bucket)) {
      for (const auto& p : bucket_list[bucket_idx]) {
        if (reference.distance(p) < min) {
          min = reference.distance(p);
          answer = p;
        }
      }
    }

    return answer;
  }
};

}  // namespace spatial
}  // namespace utec
