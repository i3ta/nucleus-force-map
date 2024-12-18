#include "opencv2/core/matx.hpp"
#include <image/image_reader.h>
#include <image/image_parse.h>
#include <iostream>
#include <nucleus_force/nucleus_force.h>
#include <unordered_map>

int main(int argc, char* argv[]) {
  if (argc == 1) {
    std::cerr << "The image path must be provided" << std::endl;
    return 1;
  }

  // Create color mapping with 3 colors:
  // - black: empty space
  // - magenta: cell
  // - green: nucleus boundary
  std::unordered_map<cv::Vec3b, int> color_mapping;
  color_mapping[cv::Vec3b(0, 0, 0)] = 0;
  color_mapping[cv::Vec3b(255, 0, 255)] = 1;
  color_mapping[cv::Vec3b(0, 255, 0)] = 2;

  nucleusforce::image::ColorMap cm(argv[1], color_mapping);

  std::cout << "Read image into color map..." << std::endl;

  std::vector<std::vector<int>> cell = nucleusforce::image::isolate_color(cm, cv::Vec3b(255, 0, 255));
  std::vector<std::vector<int>> nucleus = nucleusforce::image::isolate_color(cm, cv::Vec3b(0, 255, 0));

  std::cout << "Isolated cell and nucleus colors..." << std::endl;

  std::vector<std::vector<int>> boundary = nucleusforce::find_boundary(cell, nucleus);
  std::vector<std::vector<int>> dist = nucleusforce::find_dist(cell, nucleus);
  std::vector<std::vector<double>> force = nucleusforce::find_nucleus_force(cell, nucleus);

  std::cout << "Finding force and exporting..." << std::endl;

  nucleusforce::export_csv("output/boundary.csv", boundary);
  nucleusforce::export_csv("output/dist.csv", dist);
  nucleusforce::export_csv("output/force.csv", force);

  std::cout << "Done." << std::endl;

  return 0;
}
