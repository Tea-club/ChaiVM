cmake_minimum_required(VERSION 3.26)
project(ChaiVM)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp include/ChaiVM/interpreter/instruction.hpp include/ChaiVM/types.hpp include/ChaiVM/interpreter/executor.hpp include/ChaiVM/utils/non-copyable.hpp include/ChaiVM/interpreter/reg-file.hpp)

add_subdirectory(third_party)
add_subdirectory(test)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(include)
