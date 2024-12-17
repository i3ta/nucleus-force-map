#include <gtest/gtest.h>
#include <image/image_reader.h>
#include <stdexcept>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

TEST(ImageReaderTest, NoArgumentConstructorInitializesNothing) {
  ColorMap cm;

  ASSERT_THROW(cm.get_color_map(), std::invalid_argument);
  ASSERT_THROW(cm.get_color_mapping(), std::invalid_argument);
  ASSERT_THROW(cm.get_color_index(), std::invalid_argument);
}

TEST(ImageReaderTest, BlankImageShouldCreateColorMap) {
  fs::path image_path = fs::current_path() / "img" / "blank.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  ColorMap cm(image_path.string());
  std::vector<std::vector<int>> color_map = cm.get_color_map();

  ASSERT_EQ(color_map.size(), 16);
  ASSERT_EQ(color_map[0].size(), 16);

  std::vector<cv::Vec3b> color_mapping = cm.get_color_mapping();

  ASSERT_EQ(color_mapping.size(), 1);
  ASSERT_EQ(color_mapping[0], cv::Vec3b(255, 255, 255));

  std::unordered_map<cv::Vec3b, int> color_index = cm.get_color_index();

  ASSERT_EQ(color_index.size(), 1);
  ASSERT_EQ(color_index[cv::Vec3b(255, 255, 255)], 0);
}

TEST(ImageReaderTest, ImageWithDotShouldCreateColorMap) {
  fs::path image_path = fs::current_path() / "img" / "dot.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  ColorMap cm(image_path.string());
  std::vector<std::vector<int>> color_map = cm.get_color_map();

  ASSERT_EQ(color_map.size(), 16);
  ASSERT_EQ(color_map[0].size(), 16);

  std::vector<cv::Vec3b> color_mapping = cm.get_color_mapping();

  ASSERT_EQ(color_mapping.size(), 2);
  ASSERT_EQ(color_mapping[0], cv::Vec3b(255, 255, 255));
  ASSERT_EQ(color_mapping[1], cv::Vec3b(0, 0, 0));

  std::unordered_map<cv::Vec3b, int> color_index = cm.get_color_index();

  ASSERT_EQ(color_index.size(), 2);
  ASSERT_EQ(color_index[cv::Vec3b(255, 255, 255)], 0);
  ASSERT_EQ(color_index[cv::Vec3b(0, 0, 0)], 1);
}

TEST(ImageReaderTest, ImageWithColorsShouldCreateColorMap) {
  fs::path image_path = fs::current_path() / "img" / "colors.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  ColorMap cm(image_path.string());
  std::vector<std::vector<int>> color_map = cm.get_color_map();

  ASSERT_EQ(color_map.size(), 16);
  ASSERT_EQ(color_map[0].size(), 16);

  std::vector<cv::Vec3b> color_mapping = cm.get_color_mapping();

  ASSERT_EQ(color_mapping.size(), 8);
  ASSERT_EQ(color_mapping[0], cv::Vec3b(255, 255, 255));
  ASSERT_EQ(color_mapping[1], cv::Vec3b(0, 0, 255));
  ASSERT_EQ(color_mapping[2], cv::Vec3b(0, 255, 0));
  ASSERT_EQ(color_mapping[3], cv::Vec3b(255, 0, 0));
  ASSERT_EQ(color_mapping[4], cv::Vec3b(255, 0, 255));
  ASSERT_EQ(color_mapping[5], cv::Vec3b(255, 255, 0));
  ASSERT_EQ(color_mapping[6], cv::Vec3b(0, 255, 255));
  ASSERT_EQ(color_mapping[7], cv::Vec3b(0, 0, 0));
}

