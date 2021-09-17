#pragma once

#include "Point.h"
#include <ostream>

namespace utec {
namespace spatial {

template <typename T>
T genRandomNumber(T startRange, T endRange) {
  return startRange + (T)rand() / ((T)RAND_MAX / (T)(endRange - startRange));
}

template <typename DataType, std::size_t DIM>
struct point_comparator {
  bool operator()(const Point<DataType, DIM>& a,
                  const Point<DataType, DIM>& b) const {
    const int x = 0, y = 1;
    return (a.get(x) < b.get(x)) ||
           ((a.get(x) == b.get(x)) && (a.get(y) < b.get(y)));
  }
};

/**
 * Dataset generator
 */
class DataGenerator {
  using data_t = int;
  using point_t = Point<data_t, 2>;
  using point_set_t = std::set<point_t, point_comparator<data_t, 2>>;

 public:
  DataGenerator(std::size_t num_points, std::size_t min, std::size_t max) {
    srand((unsigned)time(0));
    for (std::size_t I = 0; I < num_points; I++)
      points.insert(point_t(
          {genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)}));
  }

  point_set_t get_data() { return points; }

  void print(std::ostream& os){
    for (auto& p : points) {
      os<<p.get(0)<<" "<<p.get(1)<<"\n";
    }
  }

 private:
  point_set_t points;
};

}  // namespace spatial
}  // namespace utec
