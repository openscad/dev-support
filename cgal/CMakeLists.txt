cmake_minimum_required(VERSION 3.15)
project(convert_to_nef)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT "$ENV{OPENSCAD_LIBRARIES}" STREQUAL "")
list(APPEND CMAKE_PREFIX_PATH "$ENV{OPENSCAD_LIBRARIES}")
if(APPLE)
  list(APPEND CMAKE_IGNORE_PREFIX_PATH "/opt/homebrew") # Homebrew Apple Silicon
  list(APPEND CMAKE_IGNORE_PREFIX_PATH "/usr/local")    # Homebrew Intel
endif()
endif()

find_package(CGAL REQUIRED)

add_executable(convert_to_nef convert_to_nef.cpp)
target_link_libraries(convert_to_nef PRIVATE CGAL::CGAL)

add_executable(decompose_to_points decompose_to_points.cpp)
target_link_libraries(decompose_to_points PRIVATE CGAL::CGAL)

add_executable(construct_nef3 construct_nef3.cpp)
target_link_libraries(construct_nef3 PRIVATE CGAL::CGAL)

add_executable(cgal-issue7271 cgal-issue7271.cpp)
target_link_libraries(cgal-issue7271 PRIVATE CGAL::CGAL)

add_executable(off_to_nef off_to_nef.cpp)
target_link_libraries(off_to_nef PRIVATE CGAL::CGAL)

add_executable(surface_mesh_to_nef surface_mesh_to_nef.cpp)
target_link_libraries(surface_mesh_to_nef PRIVATE CGAL::CGAL)
