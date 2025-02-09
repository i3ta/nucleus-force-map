#include <cassert>
#include <climits>
#include <fstream>
#include <nucleus_force/nucleus_force.h>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <utility>

#include <iostream>

namespace nucleusforce {
const int dy[8] = {0, 1, 0, -1, 0, 1, 0, -1};
const int dx[8] = {1, 0, -1, 0, 0, 1, 0, -1};

std::vector<std::vector<int>> find_dist(std::vector<std::vector<int>> cell,
                                        std::vector<std::vector<int>> nucleus) {
  if (cell.size() != nucleus.size() || cell[0].size() != nucleus[0].size()) {
    throw std::invalid_argument(
        "cell and nucleus arrays should have the same dimensions");
  }

  std::vector<std::vector<int>> dist(
      cell.size(),
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

std::vector<std::vector<int>>
find_boundary(std::vector<std::vector<int>> cell,
              std::vector<std::vector<int>> nucleus) {
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

        if (is_boundary)
          boundary[y][x] = 1;
      }
    }
  }

  return boundary;
}

std::vector<std::vector<double>>
find_nucleus_force(std::vector<std::vector<int>> cell,
                   std::vector<std::vector<int>> nucleus) {
  std::vector<std::vector<int>> boundary = find_boundary(cell, nucleus);
  std::vector<std::vector<double>> force(cell.size(),
                                         std::vector<double>(cell[0].size()));
  for (int y = 0; y < cell.size(); ++y) {
    for (int x = 0; x < cell[0].size(); ++x) {
      force[y][x] = boundary[y][x];
    }
  }

  return find_nucleus_force(cell, nucleus, force);
}

std::vector<std::vector<double>>
find_nucleus_force(std::vector<std::vector<int>> cell,
                   std::vector<std::vector<int>> nucleus,
                   std::vector<std::vector<double>> force) {
  if (cell.size() != nucleus.size() || cell.size() != force.size() ||
      cell[0].size() != nucleus[0].size() ||
      cell[0].size() != force[0].size()) {
    throw std::invalid_argument(
        "Cell, nucleus, and force array dimensions must be identical.");
  }

  std::vector<std::vector<int>> dist = find_dist(cell, nucleus);
  std::vector<std::vector<double>> f(nucleus.size(),
                                     std::vector<double>(nucleus[0].size()));
  std::priority_queue<std::pair<int, std::pair<int, int>>> q;

  for (int y = 0; y < force.size(); ++y) {
    for (int x = 0; x < force[0].size(); ++x) {
      f[y][x] = force[y][x];
      if (cell[y][x] == 1 && force[y][x] != 0) {
        q.push(make_coord(y, x, dist[y][x]));
      }
    }
  }

  while (!q.empty()) {
    int y = q.top().second.first;
    int x = q.top().second.second;
    q.pop();

    if (f[y][x] == 0)
      continue;

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

    f[y][x] = 0;
  }

  return f;
}

std::vector<std::vector<double>>
find_nucleus_force_layer(std::vector<std::vector<int>> cell,
                         std::vector<std::vector<int>> nucleus,
                         std::vector<std::vector<double>> force) {
  if (cell.size() != nucleus.size() || cell.size() != force.size() ||
      cell[0].size() != nucleus[0].size() ||
      cell[0].size() != force[0].size()) {
    throw std::invalid_argument(
        "Cell, nucleus, and force array dimensions must be identical.");
  }

  const int ROWS = cell.size();
  const int COLS = cell[0].size();

  auto removable = [&cell, &nucleus, ROWS, COLS](int r, int c) -> bool {
    // Check if pixel can be removed
    if (nucleus[r][c] ||
        ((r > 0 && cell[r - 1][c]) && (c > 0 && cell[r][c - 1]) &&
         !cell[r - 1][c - 1]) ||
        ((r < ROWS - 1 && cell[r + 1][c]) && (c < COLS - 1 && cell[r][c + 1]) &&
         !cell[r + 1][c + 1]) ||
        ((r > 0 && cell[r - 1][c]) && (c < COLS - 1 && cell[r][c + 1]) &&
         !cell[r - 1][c + 1]) ||
        ((r < ROWS - 1 && cell[r + 1][c]) && (c > 0 && cell[r][c - 1]) &&
         !cell[r + 1][c - 1]) ||
        ((r > 0 && cell[r - 1][c]) && (r < ROWS - 1 && cell[r + 1][c]) &&
         !(c > 0 && cell[r][c - 1]) && !(c < COLS - 1 && cell[r][c + 1])) ||
        ((c > 0 && cell[r][c - 1]) && (c < COLS - 1 && cell[r][c + 1]) &&
         !(r > 0 && cell[r - 1][c]) && !(r < ROWS - 1 && cell[r + 1][c])) ||
        ((c > 0 && cell[r][c - 1]) && (c < COLS - 1 && cell[r][c + 1]) &&
         (r > 0 && cell[r - 1][c]) && (r < ROWS - 1 && cell[r + 1][c]))) {
      // Skip processing this pixel
      return false;
    } else {
      return true;
    }
  };

  std::vector<std::vector<double>> f(ROWS, std::vector<double>(COLS, 0.0));
  std::queue<std::pair<int, int>> q;
  std::vector<std::vector<bool>> queued(ROWS, std::vector<bool>(COLS, false));

  // find starting points for queue and resulting force
  for (int r = 0; r < ROWS; ++r) {
    for (int c = 0; c < COLS; ++c) {
      if (force[r][c] > 0.0) {
        f[r][c] = force[r][c];
      }
      if (cell[r][c] && removable(r, c)) {
        q.push(std::make_pair(r, c));
        queued[r][c] = true;
      }
    }
  }

  // propagate force
  while (!q.empty()) {
    int r = q.front().first;
    int c = q.front().second;
    q.pop();

    if (!removable(r, c)) {
      q.push(std::make_pair(r, c));
      continue;
    }

    // set cell[r][c] = 0 to prevent propagating to itself
    cell[r][c] = 0;

    // calculate number of pixels to propagate to
    const int R_START = std::max(0, r - 1), R_END = std::min(ROWS - 1, r + 1);
    const int C_START = std::max(0, c - 1), C_END = std::min(COLS - 1, c + 1);

    int prop_count = 0;
    for (int i = R_START; i <= R_END; i++)
      for (int j = C_START; j <= C_END; j++)
        if (cell[i][j])
          prop_count++;

    if (prop_count == 0) {
      continue;
    }

    // propagate force
    double prop_val = f[r][c] / prop_count;
    for (int i = R_START; i <= R_END; i++)
      for (int j = C_START; j <= C_END; j++)
        if (cell[i][j])
          f[i][j] += prop_val;

    // continue queue
    for (int i = R_START; i <= R_END; i++)
      for (int j = C_START; j <= C_END; j++)
        if (cell[i][j] && !queued[i][j] && removable(i, j)) {
          q.push(std::make_pair(i, j));
          queued[i][j] = true;
        }
  }

  std::cout << "force" << std::endl;
  for (std::vector<double> row : f) {
    for (double i : row) {
      std::cout << i << " ";
    }
    std::cout << "\n";
  }

  return f;
}

std::vector<double>
find_nucleus_centroid(std::vector<std::vector<int>> nucleus) {
  // Find centroid of nucleus
  double mx = 0;
  double my = 0;
  int m = 0;
  for (int y = 0; y < nucleus.size(); ++y) {
    for (int x = 0; x < nucleus[0].size(); ++x) {
      if (nucleus[y][x] == 1) {
        mx += x;
        my += y;
        m++;
      }
    }
  }
  mx /= m;
  my /= m;

  return {mx, my};
}

std::vector<double> find_force_vector(std::vector<std::vector<int>> nucleus,
                                      std::vector<std::vector<double>> force) {
  std::vector<double> centroid = find_nucleus_centroid(nucleus);
  double mx = centroid[0];
  double my = centroid[1];

  // Find net force
  std::vector<double> f_net(2, 0);
  for (int y = 0; y < nucleus.size(); ++y) {
    for (int x = 0; x < nucleus[0].size(); ++x) {
      if (nucleus[y][x] && force[y][x] != 0.0) {
        double fy = my - y;
        double fx = mx - x;
        double f_mag = std::sqrt(fy * fy + fx * fx);
        fy /= f_mag; // get unit displacement vector
        fx /= f_mag;
        fy *= force[y][x];
        fx *= force[y][x];

        f_net[0] += fx;
        f_net[1] += fy;
      }
    }
  }

  return f_net;
}

inline std::pair<int, std::pair<int, int>> make_coord(int y, int x, int d) {
  return std::make_pair(d, std::make_pair(y, x));
}

void export_csv(std::string filepath, std::vector<std::vector<int>> array) {
  std::ofstream file(filepath); // Open file for writing

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
  std::ofstream file(filepath); // Open file for writing

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
