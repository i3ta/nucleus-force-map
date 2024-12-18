#include <gtest/gtest.h>
#include <nucleus_force/nucleus_force.h>
#include <image/image_reader.h>

using namespace nucleusforce;

TEST(NucleusForce_FindBoundaryTests, BlankMapHasNoBoundary) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 0, 0, 0};
  cell[2] = {0, 0, 0, 0};
  cell[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> boundary = find_boundary(cell, cell);

  ASSERT_EQ(boundary.size(), 4);
  ASSERT_EQ(boundary[0].size(), 4);

  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell[0].size(); ++x) {
      ASSERT_EQ(boundary[y][x], 0);
    }
  }
}

TEST(NucleusForce_FindBoundaryTests, MapWithDotHasBoundary) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 1, 1, 0};
  cell[2] = {0, 1, 1, 0};
  cell[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> nucleus(4, std::vector<int>(4));
  nucleus[0] = {0, 0, 0, 0};
  nucleus[1] = {0, 0, 0, 0};
  nucleus[2] = {0, 0, 0, 0};
  nucleus[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> boundary = find_boundary(cell, nucleus);

  ASSERT_EQ(boundary.size(), 4);
  ASSERT_EQ(boundary[0].size(), 4);

  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell[0].size(); ++x) {
      ASSERT_EQ(boundary[y][x], cell[y][x]);
    }
  }
}

TEST(NucleusForce_FindBoundaryTests, MapHasCellWithoutBoundary) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 1, 1, 1};
  cell[2] = {0, 1, 1, 1};
  cell[3] = {0, 1, 1, 1};

  std::vector<std::vector<int>> nucleus(4, std::vector<int>(4));
  nucleus[0] = {0, 0, 0, 0};
  nucleus[1] = {0, 0, 0, 0};
  nucleus[2] = {0, 0, 0, 0};
  nucleus[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> boundary = find_boundary(cell, nucleus);

  ASSERT_EQ(boundary.size(), 4);
  ASSERT_EQ(boundary[0].size(), 4);
  
  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell[0].size(); ++x) {
      if (y == 0 || x == 0 || x == 2 && y == 2) ASSERT_EQ(boundary[y][x], 0);
      else ASSERT_EQ(boundary[y][x], 1);
    }
  }
}

TEST(NucleusForce_FindDistTests, BlankMapHasNoDistance) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 1, 1, 0};
  cell[2] = {0, 1, 1, 0};
  cell[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> nucleus(4, std::vector<int>(4));
  nucleus[0] = {0, 0, 0, 0};
  nucleus[1] = {0, 0, 0, 0};
  nucleus[2] = {0, 0, 0, 0};
  nucleus[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> dist = find_dist(cell, nucleus);

  ASSERT_EQ(dist.size(), 4);
  ASSERT_EQ(dist[0].size(), 4);

  for (int y = 0; y < dist.size(); ++y) {
    for (int x = 0; x < dist[0].size(); ++x) {
      ASSERT_EQ(dist[y][x], -1);
    }
  }
}

TEST(NucleusForce_FindDistTests, CellWithNucleusFindsCorrectDistance) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {1, 1, 1, 1};
  cell[1] = {1, 0, 0, 1};
  cell[2] = {1, 0, 0, 1};
  cell[3] = {1, 1, 1, 1};

  std::vector<std::vector<int>> nucleus(4, std::vector<int>(4));
  nucleus[0] = {0, 0, 0, 0};
  nucleus[1] = {0, 1, 1, 0};
  nucleus[2] = {0, 1, 1, 0};
  nucleus[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> dist = find_dist(cell, nucleus);

  ASSERT_EQ(dist.size(), 4);
  ASSERT_EQ(dist[0].size(), 4);

  std::vector<std::vector<int>> true_dist(4, std::vector<int>(4));
  true_dist[0] = {2, 1, 1, 2};
  true_dist[1] = {1, 0, 0, 1};
  true_dist[2] = {1, 0, 0, 1};
  true_dist[3] = {2, 1, 1, 2};

  for (int y = 0; y < dist.size(); ++y) {
    for (int x = 0; x < dist[0].size(); ++x) {
      ASSERT_EQ(dist[y][x], true_dist[y][x]);
    }
  }
}

TEST(NucleusForce_FindDistTests, CellOnBoundaryFindsCorrectDistance) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 0, 1, 1};
  cell[2] = {0, 1, 1, 1};
  cell[3] = {0, 1, 1, 1};

  std::vector<std::vector<int>> nucleus(4, std::vector<int>(4));
  nucleus[0] = {0, 0, 0, 0};
  nucleus[1] = {0, 1, 0, 0};
  nucleus[2] = {0, 0, 0, 0};
  nucleus[3] = {0, 0, 0, 0};

  std::vector<std::vector<int>> dist = find_dist(cell, nucleus);

  ASSERT_EQ(dist.size(), 4);
  ASSERT_EQ(dist[0].size(), 4);

  std::vector<std::vector<int>> true_dist(4, std::vector<int>(4));
  true_dist[0] = {-1,-1,-1,-1};
  true_dist[1] = {-1, 0, 1, 2};
  true_dist[2] = {-1, 1, 2, 3};
  true_dist[3] = {-1, 2, 3, 4};

  for (int y = 0; y < dist.size(); ++y) {
    for (int x = 0; x < dist[0].size(); ++x) {
      ASSERT_EQ(dist[y][x], true_dist[y][x]);
    }
  }
}

TEST(NucleusForce_FindForceTests, NoCellFindsNoForce) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 0, 0, 0};
  cell[1] = {0, 0, 0, 0};
  cell[2] = {0, 0, 0, 0};
  cell[3] = {0, 0, 0, 0};

  std::vector<std::vector<double>> force = find_nucleus_force(cell, cell);

  ASSERT_EQ(force.size(), 4);
  ASSERT_EQ(force[0].size(), 4);

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      ASSERT_EQ(force[y][x], 0);
    }
  }
}

TEST(NucleusForce_FindForceTests, CellWithoutGivenForceFindsCorrectForce) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 1, 1, 1};
  cell[1] = {0, 1, 1, 1};
  cell[2] = {0, 1, 0, 1};
  cell[3] = {0, 1, 1, 1};
  
  std::vector<std::vector<int>> nucleus(4, std::vector<int>(4));
  nucleus[0] = {0, 0, 0, 0};
  nucleus[1] = {0, 0, 0, 0};
  nucleus[2] = {0, 0, 1, 0};
  nucleus[3] = {0, 0, 0, 0};

  std::vector<std::vector<double>> force = find_nucleus_force(cell, nucleus);
  
  std::vector<std::vector<double>> true_force(4, std::vector<double>(4));
  true_force[0] = {0, 0, 0, 0};
  true_force[1] = {0, 0, 0, 0};
  true_force[2] = {0, 0, 10, 0};
  true_force[3] = {0, 0, 0, 0};

  ASSERT_EQ(force.size(), 4);
  ASSERT_EQ(force[0].size(), 4);

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      ASSERT_EQ(force[y][x], true_force[y][x]);
    }
  }
}

TEST(NucleusForce_FindForceTests, CellFindsCorrectForce) {
  std::vector<std::vector<int>> cell(4, std::vector<int>(4));
  cell[0] = {0, 1, 1, 1};
  cell[1] = {0, 1, 0, 1};
  cell[2] = {0, 1, 0, 1};
  cell[3] = {0, 1, 1, 1};
  
  std::vector<std::vector<int>> nucleus(4, std::vector<int>(4));
  nucleus[0] = {0, 0, 0, 0};
  nucleus[1] = {0, 0, 1, 0};
  nucleus[2] = {0, 0, 1, 0};
  nucleus[3] = {0, 0, 0, 0};
  
  std::vector<std::vector<double>> force(4, std::vector<double>(4));
  force[0] = {0, 0, 0, 0};
  force[1] = {0, 0, 0, 0};
  force[2] = {0, 0, 0, 0};
  force[3] = {0, 1, 2, 1};

  std::vector<std::vector<double>> found_force = find_nucleus_force(cell, nucleus, force);
  
  std::vector<std::vector<double>> true_force(4, std::vector<double>(4));
  true_force[0] = {0, 0, 0, 0};
  true_force[1] = {0, 0, 0, 0};
  true_force[2] = {0, 0, 4, 0};
  true_force[3] = {0, 0, 0, 0};

  ASSERT_EQ(force.size(), 4);
  ASSERT_EQ(force[0].size(), 4);

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 4; ++x) {
      ASSERT_EQ(found_force[y][x], true_force[y][x]);
    }
  }
}
