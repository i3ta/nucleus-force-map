#include <fstream>
#include <nucleus_force/nucleus_force.h>
#include <climits>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace nucleusforce {
const int dy[8] = {0, 1, 0, -1, 0, 1, 0, -1};
const int dx[8] = {1, 0, -1, 0, 0, 1, 0, -1};

std::vector<std::vector<int>> find_dist(std::vector<std::vector<int>> cell,
                                        std::vector<std::vector<int>> nucleus) {
  if (cell.size() != nucleus.size() || cell[0].size() != nucleus[0].size()) {
    throw std::invalid_argument("cell and nucleus arrays should have the same dimensions");
  }

  std::vector<std::vector<int>> dist(cell.size(),
                                     std::vector<int>(cell[0].size(), -1)); // -1 means not reached
  std::queue<std::pair<int, std::pair<int, int>>> q;

  for (int y = 0; y < nucleus.size(); ++y) {
    for (int x = 0; x < nucleus[0].size(); ++x) {
      if (nucleus[y][x] == 1) {
        q.push(make_coord(y, x, 0));
        dist[y][x] = 0;
      }
    }
  }

  while (!q.empty()) {
    int d = q.front().first;
    int y = q.front().second.first;
    int x = q.front().second.second;
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

std::vector<std::vector<int>> find_boundary(std::vector<std::vector<int>> cell, std::vector<std::vector<int>> nucleus) {
  std::vector<std::vector<int>> boundary(cell.size(),
                                         std::vector<int>(cell[0].size()));

  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell.size(); ++x) {
      if (cell[y][x] == 1) {
        bool is_boundary = false;
        for (int i = 0; i < 8; i++) {
          int ny = y + dy[i];
          int nx = x + dx[i];
          
          if (ny < 0 || ny >= cell.size() || nx < 0 || nx >= cell[0].size() || 
            cell[ny][nx] == 0 && nucleus[ny][nx] == 0) {
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

std::vector<std::vector<double>> find_nucleus_force(std::vector<std::vector<int>> cell, 
                                                 std::vector<std::vector<int>> nucleus) {
  std::vector<std::vector<int>> boundary = find_boundary(cell, nucleus);
  std::vector<std::vector<double>> force(cell.size(),
                                         std::vector<double>(cell[0].size()));
  for (int y = 0; y < cell.size(); ++y) {
    for(int x = 0; x < cell[0].size(); ++x) {
      force[y][x] = boundary[y][x];
    }
  }

  return find_nucleus_force(cell, nucleus, force);
}

std::vector<std::vector<double>> find_nucleus_force(std::vector<std::vector<int>> cell, 
                                                 std::vector<std::vector<int>> nucleus, 
                                                 std::vector<std::vector<double>> force) {
  if (cell.size() != nucleus.size() || cell.size() != force.size() ||
    cell[0].size() != nucleus[0].size() || cell[0].size() != force[0].size()) {
    throw std::invalid_argument("Cell, nucleus, and force array dimensions must be identical.");
  }

  std::vector<std::vector<int>> dist = find_dist(cell, nucleus);
  std::vector<std::vector<double>> f(nucleus.size(), std::vector<double>(nucleus[0].size()));
  std::priority_queue<std::pair<int, std::pair<int, int>>> q;

  for (int y = 0; y < force.size(); ++y) {
    for (int x = 0; x < force[0].size(); ++x) {
      f[y][x] = force[y][x];
      if (cell[y][x] == 1 && force[y][x] != 0) q.push(make_coord(y, x, dist[y][x]));
    }
  }

  while (!q.empty()) {
    int y = q.top().second.first;
    int x = q.top().second.second;
    q.pop();

    int min_dist = INT_MAX;
    int count = 0;
    for (int i = 0; i < 4; i++) {
      int ny = y + dy[i];
      int nx = x + dx[i];
      if (ny < 0 || ny >= cell.size() || nx < 0 || nx >= cell[0].size()) {
        continue;
      }
      if (cell[ny][nx] == 1 || nucleus[ny][nx] == 1) {
        if (dist[ny][nx] >= 0 && dist[ny][nx] < min_dist) {
          min_dist = dist[ny][nx];
          count = 1;
        } else if (dist[ny][nx] == min_dist) {
          count++;
        }
      }
    }

    for (int i = 0; i < 4; i++) {
      int ny = y + dy[i];
      int nx = x + dx[i];
      if (ny < 0 || ny >= cell.size() || nx < 0 || nx >= cell[0].size()) {
        continue;
      }
      if (cell[ny][nx] == 1 || nucleus[ny][nx] == 1) {
        if (dist[ny][nx] == min_dist) {
          f[ny][nx] += (double)f[y][x] / count;
          if (nucleus[ny][nx] == 0) {
            q.push(make_coord(ny, nx, dist[ny][nx]));
          }
        }
      }
    }

    dist[y][x] = INT_MAX; // ignore this pixel from now on
    f[y][x] = 0;
  }

  return f;
}

inline std::pair<int, std::pair<int, int>> make_coord(int y, int x, int d) {
  return std::make_pair(d, std::make_pair(y, x));
}

void export_csv(std::string filepath, std::vector<std::vector<int>> array) {
  std::ofstream file(filepath);  // Open file for writing

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filepath);
  }

  for (size_t i = 0; i < array.size(); ++i) {
    std::ostringstream row;
    
    for (size_t j = 0; j < array[i].size(); ++j) {
      row << array[i][j];
      if (j < array[i].size() - 1) {
        row << ",";
      }
    }

    file << row.str() << "\n";
  }

  file.close();
}

void export_csv(std::string filepath, std::vector<std::vector<double>> array) {
  std::ofstream file(filepath);  // Open file for writing

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filepath);
  }

  for (size_t i = 0; i < array.size(); ++i) {
    std::ostringstream row;
    
    for (size_t j = 0; j < array[i].size(); ++j) {
      row << array[i][j];
      if (j < array[i].size() - 1) {
        row << ",";
      }
    }

    file << row.str() << "\n";
  }

  file.close();
}

} // namespace nucleusforce
