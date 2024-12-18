#include <gtest/gtest.h>
#include <image/image_reader.h>
#include <image/image_parse.h>
#include <filesystem>

using namespace nucleusforce::image;
namespace fs = std::filesystem;

TEST(ImageParserTest, TestParseTargetColor) {
  fs::path image_path = fs::current_path() / "img" / "dot.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  ColorMap cm(image_path.string());

  std::vector<std::vector<int>> isolated_map = isolate_color(cm, cv::Vec3b(0, 0, 0));

  ASSERT_EQ(isolated_map.size(), 16);
  ASSERT_EQ(isolated_map[0].size(), 16);

  for (int y = 0; y < isolated_map.size(); ++y) {
    for (int x = 0; x < isolated_map[0].size(); ++x) {
      if(y != 15 || x != 15) ASSERT_EQ(isolated_map[y][x], 0);
      else ASSERT_EQ(isolated_map[y][x], 1);
    }
  }
}
