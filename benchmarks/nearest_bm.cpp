#include <benchmark/benchmark.h>

#include "BasicSpatial.hpp"
#include "Point.h"
#include "Validator.h"

using namespace utec::spatial;

template <typename T>
T genRandomNumber(T startRange, T endRange) {
  return startRange + (T)rand() / ((T)RAND_MAX / (T)(endRange - startRange));
}

using data_t = int;
using point_t = Point<data_t, 2>;

template <class SpatialDS>
static void BM_nearest(benchmark::State& state) {
  const std::size_t num_points = 100000;
  const std::size_t min = 0, max = 50000 - 1;

  SpatialDS instancia;

  auto cmp = [](point_t a, point_t b) {
    const int x = 0, y = 1;
    return (a.get(x) < b.get(x)) ||
           ((a.get(x) == b.get(x)) && (a.get(y) < b.get(y)));
  };

  std::set<point_t, decltype(cmp)> points(cmp);

  for (std::size_t I = 0; I < num_points; I++)
    points.insert(point_t(
        {genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)}));

  for (auto& p : points) {
    instancia.insert(p);
  }

  point_t reference_point(
      {genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)});

  for (auto _ : state) {
    auto result = instancia.nearest_neighbor(reference_point);
  }
}

BENCHMARK_TEMPLATE(BM_nearest, Validator<point_t>);
BENCHMARK_TEMPLATE(BM_nearest, BasicSpatial<point_t>);

BENCHMARK_MAIN();
