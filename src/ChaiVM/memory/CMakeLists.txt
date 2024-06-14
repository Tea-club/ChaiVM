add_library(chai_memory STATIC)
target_sources(chai_memory PRIVATE
        ./linear-buffer.cpp
        ./free-list-trash/free-list-allocator.cpp
        ./garbage-collector.cpp
)
target_link_libraries(chai_memory
        PRIVATE
        chai_include
)
