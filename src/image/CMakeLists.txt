add_library(image image_reader.cpp)

# Find OpenCV
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include path: ${OpenCV_INCLUDE_DIRS}")

target_include_directories(image PUBLIC include)
target_include_directories(image PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(image PRIVATE ${OpenCV_LIBS})
