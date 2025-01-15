add_library(test_dependencies INTERFACE)

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
target_link_libraries(test_dependencies
  INTERFACE
  GTest::GTest
  image
  nucleus_force
)

file(COPY ${CMAKE_SOURCE_DIR}/tests/img DESTINATION ${CMAKE_BINARY_DIR}/tests)

add_executable(image_reader_test image_reader_test.cpp)
target_link_libraries(image_reader_test PRIVATE test_dependencies)

add_executable(image_parse_test image_parse_test.cpp)
target_link_libraries(image_parse_test PRIVATE test_dependencies)

add_executable(nucleus_force_test nucleus_force_test.cpp)
target_link_libraries(nucleus_force_test PRIVATE test_dependencies)

add_test(image_reader_test image_reader_test)
add_test(image_parse_test image_parse_test)
add_test(nucleus_force_test nucleus_force_test)
