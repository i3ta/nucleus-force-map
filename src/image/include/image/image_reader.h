#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <vector>

namespace std {
  /**
   * @brief Implementation of hash code for cv::Vec3b object
   */
  template <>
  struct hash<cv::Vec3b> {
    size_t operator()(const cv::Vec3b& color) const {
      return (hash<int>()(color[0]) ^ (hash<int>()(color[1]) << 1) ^ (hash<int>()(color[2]) << 2));
    }
  };
}

/**
  * @brief A class that converts a png image to a 2D color map.
  */
class ColorMap {
public:

  /**
    * @brief No-argument constructor of ColorMap
    */
  ColorMap();

  /**
    * @brief Initialize ColorMap using an image file
    *
    * @param filepath path to the png file to load the color map with
    */
  ColorMap(const std::string& filepath);

  /**
    * @brief Initialize ColorMap using an image file
    *
    * @param filepath path to the png file to load the color map with
    * @param color_mapping unordered_map containing all of the numbers that each color should map to
    */
  ColorMap(const std::string& filepath, const std::unordered_map<cv::Vec3b, int> &color_mapping);

  /**
    * @brief Load ColorMap using an image file
    *
    * @param filepath path to the png file to load the color map with
    */
  void load(const std::string& filepath);

  /**
    * @brief Load ColorMap using an image file
    *
    * @param filepath path to the png file to load the color map with
    * @param color_mapping unordered_map containing all of the numbers that each color should map to
    */
  void load(const std::string& filepath, const std::unordered_map<cv::Vec3b, int> &color_mapping);

  /**
    * @brief Get the color map associated with the image
    *
    * @return vector of vectors (2D array) with each color
    */
  const std::vector<std::vector<int>> get_color_map();

  /**
    * @brief Get the color mapping (int to color)
    *
    * @return vector of cv::Vec3b of which color each number represents
    */
  const std::unordered_map<int, cv::Vec3b> get_color_index();

  /**
    * @brief Get the color mapping (int to color)
    *
    * @return unordered map mapping colors to their respective numbers
    */
  const std::unordered_map<cv::Vec3b, int> get_color_mapping();

  /**
    * @brief recolor the color map with the provided color_mapping
    *
    * @param color_mapping unordered_map containing all of the numbers that each color should map to
    */
  void recolor(const std::unordered_map<cv::Vec3b, int> color_mapping);

  bool operator==(const ColorMap& other) const;

private:
  std::string filepath_; ///< Path to the input image file
  cv::Mat image_; ///< OpenCV matrix storing the image
  std::vector<std::vector<int>> color_map_; ///< 2D int array storing each type of pixel
  std::unordered_map<int, cv::Vec3b> color_index_; ///< Mappings from int in array to BGR color
  std::unordered_map<cv::Vec3b, int> color_mapping_; ///< int number associated with each color
};

#endif // IMAGE_READER_H
