#include <nucleus_force/nucleus_force.h>
#include <queue>
#include <stdexcept>
#include <utility>

namespace nucleusforce {
std::vector<std::vector<int>> find_dist(std::vector<std::vector<int>> cell,
                                        std::vector<std::vector<int>> nucleus) {
  if (cell.size() != nucleus.size() || cell[0].size() != nucleus[0].size()) {
    throw std::invalid_argument("cell and nucleus arrays should have the same dimensions");
  }

  const int dy[4] = {0, 1, 0, -1};
  const int dx[4] = {1, 0, -1, 0};

  std::vector<std::vector<int>> dist(cell.size(),
                                     std::vector<int>(cell[0].size(), -1)); // -1 means not reached
  std::queue<std::pair<std::pair<int, int>, int>> q;

  for (int y = 0; y < nucleus.size(); ++y) {
    for (int x = 0; x < nucleus[0].size(); ++x) {
      if (nucleus[y][x] == 1) {
        q.push(make_coord(y, x, 0));
        dist[y][x] = 0;
      }
    }
  }

  while (!q.empty()) {
    int y = q.front().first.first;
    int x = q.front().first.second;
    int d = q.front().second;
    q.pop();

    for (int i = 0; i < 4; i++) {
      int ny = y + dy[i];
      int nx = x + dx[i];
      if (ny < 0 || ny >= cell.size() || nx < 0 || nx >= cell[0].size() ||
        dist[ny][nx] != -1 || cell[ny][nx] == 0) {
        continue;
      }
      dist[ny][nx] = d + 1;
      q.push(make_coord(ny, nx, d + 1));
    }
  }

  return dist;
}

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

inline std::pair<std::pair<int, int>, int> make_coord(int y, int x, int d) {
  return std::make_pair(std::make_pair(y, x), d);
}

void export_csv(std::string filepath, std::vector<std::vector<int>> array);
}
