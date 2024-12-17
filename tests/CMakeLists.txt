add_executable(image_tests
    image_reader_test.cpp
)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

# Image input/output tests
target_link_libraries(image_tests
  PRIVATE
  GTest::GTest
  image
)

file(COPY ${CMAKE_SOURCE_DIR}/tests/img DESTINATION ${CMAKE_BINARY_DIR}/tests)

add_test(image_gtests image_tests)
