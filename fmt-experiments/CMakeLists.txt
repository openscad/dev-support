cmake_minimum_required(VERSION 3.13)

project(fmtpoc LANGUAGES CXX)

option(FMT_SUBMODULE "Use {fmt} (aka libfmt) from git submodule, as opposed to system package." ON)

add_executable(fmt-sample)

set_property(TARGET fmt-sample PROPERTY CXX_STANDARD 17)
set_property(TARGET fmt-sample PROPERTY CXX_EXTENSIONS OFF)
set_property(TARGET fmt-sample PROPERTY CXX_STANDARD_REQUIRED ON)

target_sources(fmt-sample PRIVATE main.cpp Value.h)

if(FMT_SUBMODULE)
  add_subdirectory(fmt)
else()
  find_package(fmt 9.1.0 REQUIRED)
endif()
target_link_libraries(fmt-sample PRIVATE fmt::fmt)
