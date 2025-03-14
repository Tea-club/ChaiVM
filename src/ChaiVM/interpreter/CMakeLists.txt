add_library(chai_interpreter STATIC)
target_link_libraries(chai_interpreter PRIVATE
    chai_include
)
target_sources(chai_interpreter PRIVATE
    ./executor.cpp
    ./decoder.cpp
    ./code-manager/code-manager.cpp
    ./code-manager/klass.cpp
    ./frame.cpp
    ./garbage-collector.cpp
)
