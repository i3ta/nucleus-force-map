#include <gtest/gtest.h>
#include <image/image_reader.h>
#include <stdexcept>
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

TEST(ImageReaderTest, NoArgumentConstructorInitializesNothing) {
  ColorMap cm;

  ASSERT_THROW(cm.get_color_map(), std::invalid_argument);
  ASSERT_THROW(cm.get_color_index(), std::invalid_argument);
  ASSERT_THROW(cm.get_color_mapping(), std::invalid_argument);
}

TEST(ImageReaderTest, BlankImageShouldCreateColorMap) {
  fs::path image_path = fs::current_path() / "img" / "blank.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  ColorMap cm(image_path.string());
  std::vector<std::vector<int>> color_map = cm.get_color_map();

  ASSERT_EQ(color_map.size(), 16);
  ASSERT_EQ(color_map[0].size(), 16);

  std::unordered_map<int, cv::Vec3b> color_index = cm.get_color_index();

  ASSERT_EQ(color_index.size(), 1);
  ASSERT_EQ(color_index[0], cv::Vec3b(255, 255, 255));

  std::unordered_map<cv::Vec3b, int> color_mapping = cm.get_color_mapping();

  ASSERT_EQ(color_mapping.size(), 1);
  ASSERT_EQ(color_mapping[cv::Vec3b(255, 255, 255)], 0);

  for (int r = 0; r < 16; r++) {
    for (int c = 0; c < 16; c++) {
      ASSERT_EQ(color_map[r][c], 0);
    }
  }
}

TEST(ImageReaderTest, ImageWithDotShouldCreateColorMap) {
  fs::path image_path = fs::current_path() / "img" / "dot.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  ColorMap cm(image_path.string());
  std::vector<std::vector<int>> color_map = cm.get_color_map();

  ASSERT_EQ(color_map.size(), 16);
  ASSERT_EQ(color_map[0].size(), 16);

  std::unordered_map<int, cv::Vec3b> color_index = cm.get_color_index();

  ASSERT_EQ(color_index.size(), 2);
  ASSERT_EQ(color_index[0], cv::Vec3b(255, 255, 255));
  ASSERT_EQ(color_index[1], cv::Vec3b(0, 0, 0));

  std::unordered_map<cv::Vec3b, int> color_mapping = cm.get_color_mapping();

  ASSERT_EQ(color_mapping.size(), 2);
  ASSERT_EQ(color_mapping[cv::Vec3b(255, 255, 255)], 0);
  ASSERT_EQ(color_mapping[cv::Vec3b(0, 0, 0)], 1);

  for (int r = 0; r < 16; r++) {
    for (int c = 0; c < 16; c++) {
      if (r == 15 && c == 15) ASSERT_EQ(color_map[r][c], 1);
      else ASSERT_EQ(color_map[r][c], 0);
    }
  }
}

TEST(ImageReaderTest, ImageWithColorsShouldCreateColorMap) {
  fs::path image_path = fs::current_path() / "img" / "colors.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  ColorMap cm(image_path.string());
  std::vector<std::vector<int>> color_map = cm.get_color_map();

  ASSERT_EQ(color_map.size(), 16);
  ASSERT_EQ(color_map[0].size(), 16);

  std::unordered_map<int, cv::Vec3b> color_index = cm.get_color_index();

  ASSERT_EQ(color_index.size(), 8);
  ASSERT_EQ(color_index[0], cv::Vec3b(255, 255, 255));
  ASSERT_EQ(color_index[1], cv::Vec3b(0, 0, 255));
  ASSERT_EQ(color_index[2], cv::Vec3b(0, 255, 0));
  ASSERT_EQ(color_index[3], cv::Vec3b(255, 0, 0));
  ASSERT_EQ(color_index[4], cv::Vec3b(255, 0, 255));
  ASSERT_EQ(color_index[5], cv::Vec3b(255, 255, 0));
  ASSERT_EQ(color_index[6], cv::Vec3b(0, 255, 255));
  ASSERT_EQ(color_index[7], cv::Vec3b(0, 0, 0));
}

TEST(ImageReaderTest, RecolorImagesMissingColorsShouldThrowError) {
  fs::path image_path = fs::current_path() / "img" / "colors.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  std::unordered_map<cv::Vec3b, int> color_mapping;
  color_mapping[cv::Vec3b(255, 255, 255)] = 2;

  ColorMap cm(image_path.string());
  ASSERT_THROW(cm.recolor(color_mapping), std::invalid_argument);
}

TEST(ImageReaderTest, RecolorImagesRepeatedColorsShouldThrowError) {
  fs::path image_path = fs::current_path() / "img" / "colors.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  std::unordered_map<cv::Vec3b, int> color_mapping;
  color_mapping[cv::Vec3b(0, 0, 0)] = 2;
  color_mapping[cv::Vec3b(255, 255, 255)] = 2;

  ColorMap cm(image_path.string());
  ASSERT_THROW(cm.recolor(color_mapping), std::invalid_argument);
}

TEST(ImageReaderTest, RecolorImageShouldRenumberColorMap) {
  fs::path image_path = fs::current_path() / "img" / "dot.png";

  ASSERT_TRUE(fs::exists(image_path)) << "Test image file does not exist at path: " << image_path;

  std::unordered_map<cv::Vec3b, int> color_mapping;
  color_mapping[cv::Vec3b(0, 0, 0)] = 11;
  color_mapping[cv::Vec3b(255, 255, 255)] = 10;

  ColorMap cm(image_path.string());
  ASSERT_NO_THROW(cm.recolor(color_mapping));

  std::vector<std::vector<int>> color_map = cm.get_color_map();
  std::unordered_map<int, cv::Vec3b> color_index = cm.get_color_index();

  ASSERT_EQ(color_index.size(), 2);
  ASSERT_EQ(color_index[10], cv::Vec3b(255, 255, 255));
  ASSERT_EQ(color_index[11], cv::Vec3b(0, 0, 0));

  ASSERT_EQ(color_mapping.size(), 2);
  ASSERT_EQ(color_mapping[cv::Vec3b(0, 0, 0)], 11);
  ASSERT_EQ(color_mapping[cv::Vec3b(255, 255, 255)], 10);
  for (int r = 0; r < 16; r++) {
    for (int c = 0; c < 16; c++) {
      if (r == 15 && c == 15) ASSERT_EQ(color_map[r][c], 11);
      else ASSERT_EQ(color_map[r][c], 10);
    }
  }
}
