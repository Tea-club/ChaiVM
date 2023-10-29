
add_library(chai_testif INTERFACE)
target_link_libraries(chai_testif INTERFACE
    chai_include
    chai_interpreter
    chai_utils
    gmock_main
    gmock
    gtest
)

function(chai_test SOURCE_NAME)
    get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
    target_link_libraries(${TARGET_NAME} PRIVATE chai_testif)
    add_test(NAME ${TARGET_NAME} COMMAND ./${TARGET_NAME})
endfunction(chai_test)

include (CTest)
chai_test(./dummy_test.cpp)

add_subdirectory(ChaiVM)
add_subdirectory(frontend)
