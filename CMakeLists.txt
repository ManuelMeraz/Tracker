# 3.9.6 for C++ 17
cmake_minimum_required(VERSION 3.9.6...3.13)

# TODO(Manny): Use git tag to get version
project(tracker VERSION 0.0.1 LANGUAGES CXX)

# Build Options
option(ENABLE_DOCUMENTATION "Build tracker documentation" OFF)
option(ENABLE_TESTS "Build tests" OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Sets compiler standards(e.g. C++ 17) and checks cmake version
include(${CMAKE_MODULE_PATH}/SetupTracker.cmake)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Creates libraries and creates main executable
add_subdirectory(src)

if(ENABLE_DOCUMENTATION)
  add_subdirectory(docs)
endif()

if(ENABLE_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

install(DIRECTORY ${CMAKE_BINARY_DIR}/lib/tracker DESTINATION /usr/local/lib)
