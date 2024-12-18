#include <image/image_parse.h>

#include <unordered_map>
#include <vector>

namespace nucleusforce::image {
std::vector<std::vector<int>> isolate_color(ColorMap cm, cv::Vec3b color) {
  std::vector<std::vector<int>> complete_map = cm.get_color_map();
  std::vector<std::vector<int>> isolated_map(complete_map.size(), 
                                             std::vector<int>(complete_map[0].size(), 0));
  const std::unordered_map<cv::Vec3b, int> color_mapping = cm.get_color_mapping();
  auto color_it = color_mapping.find(color);
  if (color_it == color_mapping.end()) {
    return isolated_map;
  }
  int target_color = color_it->second;

  for (int y = 0; y < complete_map.size(); ++y) {
    for (int x = 0; x < complete_map[0].size(); ++x) {
      isolated_map[y][x] = complete_map[y][x] == target_color;
    }
  }

  return isolated_map;
}
}
