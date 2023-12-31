
add_library(chai_testif INTERFACE)
target_link_libraries(chai_testif INTERFACE
    chai_include
    chai_interpreter
    chai_utils
    front_asm
    chai_memory
    gmock_main
    gmock
    gtest
)

function(chai_test SOURCE_NAME)
    get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
    target_link_libraries(${TARGET_NAME} PRIVATE chai_testif)
    if (CHAIVM_ADD_MEM_SANITIZER AND NOT CHAIVM_ADD_THREAD_SANITIZER)
        # @todo #37:60min Make address and leak and undefined sanitizer work on MacOS with ARM
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address -g -fsanitize=leak -fsanitize=undefined)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address -fsanitize=leak  -fsanitize=undefined)
    elseif(CHAIVM_ADD_THREAD_SANITIZER AND NOT CHAIVM_ADD_MEM_SANITIZER)
        # @todo #37:60min Make thread sanitizer work on MacOS with ARM
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=thread -g)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=thread)
    endif()
    add_test(NAME ${TARGET_NAME} COMMAND ./${TARGET_NAME})
endfunction(chai_test)

include (CTest)
chai_test(./dummy_test.cpp)

add_subdirectory(ChaiVM)
add_subdirectory(frontend)
