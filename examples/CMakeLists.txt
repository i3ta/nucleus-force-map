add_executable(eg_boundary_force eg_boundary_force.cpp)

target_link_libraries(eg_boundary_force PRIVATE image nucleus_force)

file(COPY ${CMAKE_SOURCE_DIR}/examples/img DESTINATION ${CMAKE_BINARY_DIR}/examples)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/examples/output)
