#include "opencv2/core/matx.hpp"
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <image/image_parse.h>
#include <image/image_reader.h>
#include <iostream>
#include <nucleus_force/nucleus_force.h>
#include <unordered_map>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "The image path must be provided" << std::endl;
    return 1;
  }

  const int ITERATIONS = argc >= 3 ? atoi(argv[2]) : 100;

  // Create color mapping with 3 colors:
  // - Empty space
  // - Cell
  // - Nucleus
  // - Origins of force
  std::unordered_map<cv::Vec3b, int> color_mapping;
  color_mapping[cv::Vec3b(0, 0, 0)] = 0;
  color_mapping[cv::Vec3b(147, 78, 71)] = 1;   // cell
  color_mapping[cv::Vec3b(169, 186, 114)] = 2; // nucleus
  color_mapping[cv::Vec3b(181, 231, 213)] = 3; // origins of force

  nucleusforce::image::ColorMap cm(argv[1], color_mapping);

  std::cout << "Read image into color map..." << std::endl;

  std::vector<std::vector<int>> cell =
      nucleusforce::image::isolate_color(cm, cv::Vec3b(147, 78, 71));
  std::vector<std::vector<int>> nucleus =
      nucleusforce::image::isolate_color(cm, cv::Vec3b(169, 186, 114));
  std::vector<std::vector<int>> force_origin =
      nucleusforce::image::isolate_color(cm, cv::Vec3b(181, 231, 213));

  std::vector<std::vector<double>> init_force(
      force_origin.size(), std::vector<double>(force_origin[0].size()));

  int cell_size = 0;

  std::cout << "Isolated cell and nucleus colors..." << std::endl;

  // correct cell size
  for (int r = 0; r < cell.size(); ++r) {
    for (int c = 0; c < cell[0].size(); ++c) {
      if (nucleus[r][c] || force_origin[r][c]) {
        cell[r][c] = 1;
      }
      if (cell[r][c]) {
        cell_size++;
      }
      if (force_origin[r][c]) {
        init_force[r][c] = 1.0; // set force from each pixel to be 1.0
      }
    }
  }

  std::cout << "Running tests with " << ITERATIONS << " iterations...\n"
            << std::endl;

  // Run tests with min dist method
  long long us_min_dist = 0;
  std::vector<long long> times_min_dist(ITERATIONS);
  for (int t = 0; t < ITERATIONS; ++t) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<double>> force_min_dist =
        nucleusforce::find_nucleus_force(cell, nucleus, init_force);

    auto elapsed_min_dist = std::chrono::high_resolution_clock::now() - start;
    times_min_dist[t] =
        std::chrono::duration_cast<std::chrono::microseconds>(elapsed_min_dist)
            .count();
    us_min_dist += times_min_dist[t];
  }

  // Run tests with layer method
  long long us_layer = 0;
  std::vector<long long> times_layer(ITERATIONS);
  for (int t = 0; t < ITERATIONS; ++t) {
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<double>> force_layer =
        nucleusforce::find_nucleus_force_layer(cell, nucleus, init_force);

    auto elapsed_layer = std::chrono::high_resolution_clock::now() - start;
    times_layer[t] =
        std::chrono::duration_cast<std::chrono::microseconds>(elapsed_layer)
            .count();
    us_layer += times_layer[t];
  }

  // Calculate std. dev
  double avg_min_dist = (double)us_min_dist / ITERATIONS;
  double std_dev_min_dist = 0.0;
  for (long long t : times_min_dist) {
    std_dev_min_dist += (t - avg_min_dist) * (t - avg_min_dist);
  }
  std_dev_min_dist /= ITERATIONS - 1;
  std_dev_min_dist = sqrt(std_dev_min_dist);

  double avg_layer = (double)us_layer / ITERATIONS;
  double std_dev_layer = 0.0;
  for (long long t : times_layer) {
    std_dev_layer += (t - avg_layer) * (t - avg_layer);
  }
  std_dev_layer /= ITERATIONS - 1;
  std_dev_layer = sqrt(std_dev_layer);

  // Report results
  std::cout << "Test Results\n-----\n";
  std::cout << "Cell pixel count: " << cell_size << "\n";

  std::cout << std::fixed << std::setprecision(3);

  std::cout << "The min dist method had an average runtime of " << avg_min_dist
            << "μs (" << avg_min_dist / cell_size
            << "μs per cell pixel), with a standard deviation of "
            << std_dev_min_dist << "μs." << std::endl;

  std::cout << "The layer method had an average runtime of " << avg_layer
            << "μs (" << avg_layer / cell_size
            << "μs per cell pixel), with a standard deviation of "
            << std_dev_layer << "μs." << std::endl;

  if (avg_min_dist < avg_layer) {
    std::cout << "The min dist method is "
              << (double)(avg_layer - avg_min_dist) / avg_min_dist * 100
              << "% faster." << std::endl;
  } else if (avg_layer < avg_min_dist) {
    std::cout << "The layer method is "
              << (double)(avg_min_dist - avg_layer) / avg_layer * 100
              << "% faster." << std::endl;
  }
}
