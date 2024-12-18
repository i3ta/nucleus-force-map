#include <nucleus_force/nucleus_force.h>

namespace nucleusforce {
std::vector<std::vector<int>> find_dist(std::vector<std::vector<int>> cell,
                                        std::vector<std::vector<int>> nucleus);

std::vector<std::vector<int>> find_boundary(std::vector<std::vector<int>> cell) {
  const int dy[8] = {0, 1, 0, -1, 0, 1, 0, -1};
  const int dx[8] = {1, 0, -1, 0, 0, 1, 0, -1};

  std::vector<std::vector<int>> boundary(cell.size(),
                                         std::vector<int>(cell[0].size()));

  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell.size(); ++x) {
      if (cell[y][x] == 1) {
        bool is_boundary = false;
        for (int i = 0; i < 8; i++) {
          int ny = y + dy[i];
          int nx = x + dx[i];
          
          if (ny >= 0 && ny < cell.size() && nx >= 0 && nx < cell[0].size() && cell[ny][nx] == 0) {
            is_boundary = true;
            break;
          }
        }

        if (is_boundary) boundary[y][x] = 1;
      }
    }
  }

  return boundary;
}

std::vector<std::vector<int>> find_nucleus_force(std::vector<std::vector<int>> cell);

std::vector<std::vector<int>> find_nucleus_force(std::vector<std::vector<int>> cell, std::vector<std::vector<int>> force);

void export_csv(std::string filepath, std::vector<std::vector<int>> array);
}
