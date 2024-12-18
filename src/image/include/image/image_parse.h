#ifndef IMAGE_PARSE_H
#define IMAGE_PARSE_H

#include <image/image_reader.h>
#include <opencv2/core.hpp>
#include <vector>

namespace nucleusforce::image {
/**
  * @brief Isolates a single color in the color map
  *
  * @return 2D array (vector of vectors) where the target color is filled
  *         with 1 and everything else is 0
  */
std::vector<std::vector<int>> isolate_color(ColorMap cm, cv::Vec3b color);
}

#endif
