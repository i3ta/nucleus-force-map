#include <gtest/gtest.h>
#include <nucleus_force/nucleus_force.h>
#include <image/image_reader.h>

using namespace nucleusforce;

TEST(NucleusForceTests, BlankMapHasNoBoundary) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 0, 0, 0};
  cell[2] = {0, 0, 0, 0};
  cell[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> boundary = find_boundary(cell);

  ASSERT_EQ(boundary.size(), 4);
  ASSERT_EQ(boundary[0].size(), 4);

  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell[0].size(); ++x) {
      ASSERT_EQ(boundary[y][x], 0);
    }
  }
}

TEST(NucleusForceTests, MapWithDotHasBoundary) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 1, 1, 0};
  cell[2] = {0, 1, 1, 0};
  cell[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> boundary = find_boundary(cell);

  ASSERT_EQ(boundary.size(), 4);
  ASSERT_EQ(boundary[0].size(), 4);

  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell[0].size(); ++x) {
      ASSERT_EQ(boundary[y][x], cell[y][x]);
    }
  }
}

TEST(NucleusForceTests, MapHasCellWithoutBoundary) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 1, 1, 1};
  cell[2] = {0, 1, 1, 1};
  cell[3] = {0, 1, 1, 1};

  std::vector<std::vector<int>> boundary = find_boundary(cell);

  ASSERT_EQ(boundary.size(), 4);
  ASSERT_EQ(boundary[0].size(), 4);
  
  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell[0].size(); ++x) {
      if (y > 0 && x > 0 && (y == 1 || x == 1)) ASSERT_EQ(boundary[y][x], 1);
      else ASSERT_EQ(boundary[y][x], 0);
    }
  }
}
