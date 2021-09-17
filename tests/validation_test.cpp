#include <gtest/gtest.h>

#include <iostream>

#include "Point.h"
#include "Validator.h"
#include "BasicGridSpatial.hpp"

using namespace utec::spatial;

template <typename T>
T genRandomNumber(T startRange, T endRange) {
  return startRange + (T)rand() / ((T)RAND_MAX / (T)(endRange - startRange));
}

TEST(SimpleTest, basicTest) {
  using data_t = int;
  using point_t = Point<data_t, 2>;

  const std::size_t num_points = 100000;
  const std::size_t min = 0, max = 50000-1;

  Validator<point_t> validator;
  BasicGridSpatial<point_t> instancia(max+1, 10);

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
    validator.insert(p);
    instancia.insert(p);
  }

  for (std::size_t J = 0; J < 10; J++) {
    point_t reference_point({genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)});

    auto reference_result = validator.nearest_neighbor(reference_point);
    auto result = instancia.nearest_neighbor(reference_point);

    EXPECT_EQ(reference_point.distance(reference_result), reference_point.distance(result));
  }
}

int main(int argc, char** argv) {
  srand((unsigned)time(0));

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}