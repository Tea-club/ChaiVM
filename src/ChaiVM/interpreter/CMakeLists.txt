add_library(chai_interpreter STATIC)
target_link_libraries(chai_interpreter PRIVATE
    chai_include
)
target_sources(chai_interpreter PRIVATE
    ./reg-file.cpp
    ./executor.cpp
    ./decoder.cpp
)
