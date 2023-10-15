add_library(chai_benchif INTERFACE)
target_link_libraries(chai_benchif INTERFACE
    chai_include
    chai_interpreter
    benchmark
)

add_executable(bench main_bench.cpp)
function(chai_bench SOURCE_NAME)
    get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
    target_sources(bench PRIVATE ${SOURCE_NAME})
endfunction(chai_bench)

chai_bench(dummy_bench.cpp)
chai_bench(interpreter_bench.cpp)
target_link_libraries(bench PRIVATE chai_benchif)

add_custom_target(run_bench
    COMMENT "running all benchs"
    COMMAND ./bench
)
add_dependencies(run_bench bench)
