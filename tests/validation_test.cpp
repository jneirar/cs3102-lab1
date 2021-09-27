#include <gtest/gtest.h>

#include <iostream>

#include "BasicSpatial.hpp"
#include "Point.h"
#include "RangeBST.hpp"
#include "RangeTree1D.hpp"
#include "Validator.h"
#include "utils/DataGenerator.hpp"

using namespace utec::spatial;

using data_t = double;
using point_t = Point<data_t, 1>;

template <typename TypeED>
class RangeTest : public ::testing::Test {
 protected:
  TypeED instancia;
};

using SpatialTypes = ::testing::Types<BasicSpatial<point_t>, RangeBST<point_t>,
                                      RangeTree1D<point_t>>;

TYPED_TEST_SUITE_P(RangeTest);

TYPED_TEST_P(RangeTest, BasicTest) {
  const std::size_t num_points = 10000;
  const data_t min = 0, max = 1000;

  DataGenerator1D<data_t> generator(num_points, min, max);
  auto points = generator.get_data();

  point_t query_min = point_t({max * 0.2});
  point_t query_max = point_t({max * 0.8});

  Validator<point_t> validator;

  for (auto& p : points) {
    this->instancia.insert(p);
    validator.insert(p);
  }

  auto reference_result = validator.range(query_min, query_max);
  auto result = this->instancia.range(query_min, query_max);

  EXPECT_EQ(reference_result.size(), result.size());

  std::sort(result.begin(), result.end());
  std::sort(reference_result.begin(), reference_result.end());

  EXPECT_EQ(reference_result, result);
}

REGISTER_TYPED_TEST_SUITE_P(RangeTest, BasicTest);

INSTANTIATE_TYPED_TEST_SUITE_P(RangeSuite, RangeTest, SpatialTypes);

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}