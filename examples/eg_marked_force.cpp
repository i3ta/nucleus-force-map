#include "opencv2/core/matx.hpp"
#include <image/image_parse.h>
#include <image/image_reader.h>
#include <iostream>
#include <nucleus_force/nucleus_force.h>
#include <unordered_map>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cerr << "The image path must be provided" << std::endl;
    return 1;
  }

  // Create color mapping with 4 colors:
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

  // correct cell size
  for (int r = 0; r < cell.size(); ++r) {
    for (int c = 0; c < cell[0].size(); ++c) {
      if (nucleus[r][c] || force_origin[r][c]) {
        cell[r][c] = 1;
      }
      if (force_origin[r][c]) {
        init_force[r][c] = 1.0; // set force from each pixel to be 1.0
      }
    }
  }

  std::cout << "Isolated cell and nucleus colors..." << std::endl;

  std::vector<std::vector<int>> boundary =
      nucleusforce::find_boundary(cell, nucleus);
  std::vector<std::vector<int>> dist = nucleusforce::find_dist(cell, nucleus);
  std::vector<std::vector<double>> force =
      nucleusforce::find_nucleus_force_layer(cell, nucleus, init_force);

  std::cout << "Finding force and exporting..." << std::endl;

  nucleusforce::export_csv("output/boundary.csv", boundary);
  nucleusforce::export_csv("output/dist.csv", dist);
  nucleusforce::export_csv("output/force.csv", force);

  std::cout << "Done." << std::endl;

  return 0;
}
