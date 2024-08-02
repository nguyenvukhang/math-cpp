cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0135 NEW)

project(MinimathProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
if(NOT WIN32)
  string(ASCII 27 x)
  set(ColourReset "${x}[m")
  set(Cyan        "${x}[36m")
  set(Gray        "${x}[37m")
endif()
message("${Gray}CMake Build Type: [${Cyan}${CMAKE_BUILD_TYPE}${Gray}]${ColourReset}")

set(SOURCE_FILES 
  label.cc
  fs.cc
)

# Test binary builds
if("${CMAKE_BUILD_TYPE}" STREQUAL "Test")
  # Grab Google Test suite
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  FetchContent_MakeAvailable(googletest)

  enable_testing()
  add_executable(minimath_test test.cc ${SOURCE_FILES})
  target_link_libraries(minimath_test GTest::gtest_main)

  include(GoogleTest)
  gtest_discover_tests(minimath_test)
else()
  add_executable(minimath main.cc ${SOURCE_FILES})
  target_include_directories(minimath PRIVATE deps)
endif()

unset(CMAKE_BUILD_TYPE CACHE)
