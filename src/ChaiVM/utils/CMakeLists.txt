add_library(chai_utils STATIC)
target_sources(chai_utils PRIVATE
    ./file-format/chai-file.cpp
    ./file-format/function-info.cpp
    ./instr2Raw.cpp
)
target_link_libraries(chai_utils
PRIVATE
    chai_include
)
