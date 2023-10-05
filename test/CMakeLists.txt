
add_library(chaivm_testif INTERFACE)
target_link_libraries(chaivm_testif INTERFACE
    gmock_main
    gmock
    gtest
)

function(chaivm_test SOURCE_NAME)
    get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE ${SOURCE_NAME})
    target_link_libraries(${TARGET_NAME} PRIVATE chaivm_testif)
    add_test(NAME ${TARGET_NAME} COMMAND ./${TARGET_NAME})
endfunction(chaivm_test)

include (CTest)
chaivm_test(./dummy_test.cpp)
