#include <gtest/gtest.h>

#include <iostream>

#include "Point.h"
#include "Validator.h"
#include "BasicGridSpatial.hpp"
#include "utils/DataGenerator.hpp"
#include <fstream>

using namespace utec::spatial;

TEST(SimpleTest, basicTest) {
  using data_t = int;
  using point_t = Point<data_t, 2>;

  const std::size_t num_points = 100000;
  const std::size_t min = 0, max = 50000-1;

  Validator<point_t> validator;
  BasicGridSpatial<point_t> instancia(max+1, 10);

  DataGenerator generator(num_points, min, max);
  auto points = generator.get_data();

  for (auto& p : points) {
    validator.insert(p);
    instancia.insert(p);
  }

  std::ofstream dataset;
  dataset.open("dataset.txt");
  generator.print(dataset);
  dataset.close();

  point_t reference_point({genRandomNumber<int>(min, max), genRandomNumber<int>(min, max)});

  auto reference_result = validator.nearest_neighbor(reference_point);
  auto result = instancia.nearest_neighbor(reference_point);

  std::ofstream output;
  output.open("output.txt");
  output<<reference_point.get(0)<<" "<<reference_point.get(1)<<" "<<0<<"\n";
  output<<reference_result.get(0)<<" "<<reference_result.get(1)<<" 1"<<"\n";
  output<<result.get(0)<<" "<<result.get(1)<<" 2"<<"\n";
  output.close();

  EXPECT_EQ(reference_point.distance(reference_result), reference_point.distance(result));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}