#include <image/image_reader.h>

#include <opencv2/core/matx.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdexcept>
#include <unordered_map>
#include <vector>

ColorMap::ColorMap() {
  return;
}

ColorMap::ColorMap(const std::string& filepath) {
  load(filepath);
}

ColorMap::ColorMap(const std::string& filepath, const std::unordered_map<cv::Vec3b, int> &color_mapping) {
  load(filepath, color_mapping);
}

void ColorMap::load(const std::string& filepath) {
  cv::Mat image = cv::imread(filepath, cv::IMREAD_COLOR);
  if (image.empty()) {
    throw std::invalid_argument("Could not load the image at: " + filepath);
  }

  // Create color map
  std::vector<std::vector<int>> color_map(image.rows,
                                          std::vector<int>(image.cols));
  std::unordered_map<int, cv::Vec3b> color_index;
  std::unordered_map<cv::Vec3b, int> color_mapping;

  int index = 0;
  for (int y = 0; y < image.rows; ++y) {
    for (int x = 0; x < image.cols; ++x) {
      cv::Vec3b color = image.at<cv::Vec3b>(y, x);
      if (color_mapping.find(color) == color_mapping.end()) {
        color_index[index++] = color;
        color_mapping[color] = color_index.size() - 1;
      }
      color_map[y][x] = color_mapping[color];
    }
  }

  // Assign color map to variables
  filepath_ = filepath;
  image_ = image;
  color_map_ = color_map;
  color_index_ = color_index;
  color_mapping_ = color_mapping;
}

void ColorMap::load(const std::string& filepath, const std::unordered_map<cv::Vec3b, int> &color_mapping) {
  load(filepath);
  recolor(color_mapping);
}

void ColorMap::recolor(const std::unordered_map<cv::Vec3b, int> color_mapping) {
  if (image_.empty()) {
    throw std::invalid_argument("Image must be loaded before recoloring color map");
  }

  std::unordered_map<int, cv::Vec3b> color_index;
  for (auto color : color_mapping) {
    if (color_index.find(color.second) != color_index.end()) {
      throw std::invalid_argument("color_mapping cannot contain any repeated numbers");
    }
    color_index[color.second] = color.first;
  }

  std::vector<std::vector<int>> color_map(image_.rows,
                                          std::vector<int>(image_.cols));
  for (int y = 0; y < image_.rows; ++y) {
    for (int x = 0; x < image_.cols; ++x) {
      cv::Vec3b color = image_.at<cv::Vec3b>(y, x);
      auto it = color_mapping.find(color);
      if (it == color_mapping.end()) {
        throw std::invalid_argument("Missing color: (" + std::to_string(color[0]) + "," +
                                    std::to_string(color[1]) + "," + std::to_string(color[2]) + ")");
      }
      color_map[y][x] = it->second;
    }
  }

  color_mapping_ = color_mapping;
  color_map_ = color_map;
  color_index_ = color_index;
}

const std::vector<std::vector<int>> ColorMap::get_color_map() {
  if (filepath_.empty()) {
    throw std::invalid_argument("Image must be loaded before getting color map.");
  }
  return color_map_;
}

const std::unordered_map<int, cv::Vec3b> ColorMap::get_color_index() {
  if (filepath_.empty()) {
    throw std::invalid_argument("Image must be loaded before getting color mapping.");
  }
  return color_index_;
}

const std::unordered_map<cv::Vec3b, int> ColorMap::get_color_mapping() {
  if (filepath_.empty()) {
    throw std::invalid_argument("Image must be loaded before getting color mapping.");
  }
  return color_mapping_;
}
