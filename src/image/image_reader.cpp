#include <image/image_reader.h>

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

ColorMap::ColorMap(const std::string& filepath, const std::unordered_map<cv::Vec3b, int> &color_index) {
  load(filepath, color_index);
}

void ColorMap::load(const std::string& filepath) {
  cv::Mat image = cv::imread(filepath, cv::IMREAD_COLOR);
  if (image.empty()) {
    throw std::invalid_argument("Could not load the image at: " + filepath);
  }


  // Create color map
  std::vector<std::vector<int>> color_map(image.rows,
                                          std::vector<int>(image.cols));
  std::vector<cv::Vec3b> color_mapping;
  std::unordered_map<cv::Vec3b, int> color_index;

  for (int y = 0; y < image.rows; ++y) {
    for (int x = 0; x < image.cols; ++x) {
      cv::Vec3b color = image.at<cv::Vec3b>(y, x);
      if (color_index.find(color) == color_index.end()) {
        color_mapping.push_back(color);
        color_index[color] = color_mapping.size() - 1;
      }
      color_map[y][x] = color_index[color];
    }
  }

  // Assign color map to variables
  filepath_ = filepath;
  image_ = image;
  color_map_ = color_map;
  color_mapping_ = color_mapping;
  color_index_ = color_index;
}

void ColorMap::load(const std::string& filepath, const std::unordered_map<cv::Vec3b, int> &color_index) {
  load(filepath);
  recolor(color_index);
}

void ColorMap::recolor(const std::unordered_map<cv::Vec3b, int> color_index) {
  if (image_.empty()) {
    throw std::invalid_argument("Image must be loaded before recoloring color map");
  }

  std::vector<std::vector<int>> color_map(image_.rows,
                                          std::vector<int>(image_.cols));
  for (int y = 0; y < image_.rows; ++y) {
    for (int x = 0; x < image_.cols; ++x) {
      cv::Vec3b color = image_.at<cv::Vec3b>(y, x);
      auto it = color_index.find(color);
      if (it == color_index.end()) {
        throw std::invalid_argument("Missing color: (" + std::to_string(color[0]) + "," +
                                    std::to_string(color[1]) + "," + std::to_string(color[2]) + ")");
      }
      color_map[y][x] = it->second;
    }
  }

  color_index_ = color_index;
  color_map_ = color_map;
}

const std::vector<std::vector<int>> ColorMap::get_color_map() {
  if (filepath_.empty()) {
    throw std::invalid_argument("Image must be loaded before getting color map.");
  }
  return color_map_;
}

const std::vector<cv::Vec3b> ColorMap::get_color_mapping() {
  if (filepath_.empty()) {
    throw std::invalid_argument("Image must be loaded before getting color mapping.");
  }
  return color_mapping_;
}

const std::unordered_map<cv::Vec3b, int> ColorMap::get_color_index() {
  if (filepath_.empty()) {
    throw std::invalid_argument("Image must be loaded before getting color mapping.");
  }
  return color_index_;
}

bool ColorMap::operator==(const ColorMap& other) const {
  return filepath_ == other.filepath_ &&
         image_.total() == other.image_.total() &&
         image_.type() == other.image_.type() &&
         cv::countNonZero(image_ != other.image_) == 0 &&
         color_map_ == other.color_map_ &&
         color_mapping_ == other.color_mapping_ &&
         color_index_ == other.color_index_;
}
