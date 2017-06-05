cmake_minimum_required(VERSION 3.7.0)
project(requests_cpp VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

include_directories(src/include)

file(GLOB LIB_SRC_FILE ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(requests ${LIB_SRC_FILE})

add_executable(test_main test/test.cpp)
target_link_libraries(test_main requests curl)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
