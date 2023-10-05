cmake_minimum_required(VERSION 3.26)
project(ChaiVM)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp)

add_subdirectory(third_party)
add_subdirectory(test)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
