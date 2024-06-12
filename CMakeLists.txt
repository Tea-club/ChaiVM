cmake_minimum_required(VERSION 3.26)
project(ChaiVM)
set(CMAKE_CXX_STANDARD 20)

set(COMPILER_WARNINGS "-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-missing-field-initializers -Wno-sign-compare")
set(CMAKE_C_FLAGS ${COMPILER_WARNINGS})
set(CMAKE_C_FLAGS_DEBUG "-O0 -g ${COMPILER_WARNINGS}")
set(CMAKE_C_FLAGS_RELEASE "-O2 ${COMPILER_WARNINGS}")
set(CMAKE_CXX_FLAGS ${COMPILER_WARNINGS})
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g ${COMPILER_WARNINGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 ${COMPILER_WARNINGS}")

add_subdirectory(third_party)
add_subdirectory(test)
add_subdirectory(src)
if (CHAI_BENCH)
    add_subdirectory(bench)
endif ()

# @todo #15:90min Generate autogen/operations.hpp into build directory.
set(OPERATIONS_DIR ${CMAKE_SOURCE_DIR}/include/ChaiVM/interpreter/autogen/operations.hpp)
add_custom_target(gen_operations_header
        COMMENT "Generating ${OPERATIONS_DIR}"
        OUTPUT ${OPERATIONS_DIR}
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/opcode2operation-generator.py ${OPERATIONS_DIR} ${CMAKE_SOURCE_DIR}/tools/resources/instructions.yml
)

add_library(chai_include INTERFACE)
add_dependencies(chai_include gen_operations_header)
target_include_directories(chai_include INTERFACE
    include
)

add_executable(ChaiVM src/main.cpp)
target_link_libraries(ChaiVM PUBLIC
    chai_include
    chai_interpreter
)
