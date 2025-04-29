cmake_minimum_required(VERSION 3.15)
project(convert_to_nef)

find_package(CGAL REQUIRED)

add_executable(convert_to_nef convert_to_nef.cpp)
target_link_libraries(convert_to_nef PRIVATE CGAL::CGAL)

set_target_properties(convert_to_nef PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
