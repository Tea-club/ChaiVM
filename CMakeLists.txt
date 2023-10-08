cmake_minimum_required(VERSION 3.26)
project(ChaiVM)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third_party)
add_subdirectory(test)
add_subdirectory(src)

add_library(chai_include INTERFACE)
target_include_directories(chai_include INTERFACE
    include
)

include_directories(include)

add_executable(ChaiVM src/main.cpp)
target_link_libraries(ChaiVM PUBLIC
    chai_interpreter
)
