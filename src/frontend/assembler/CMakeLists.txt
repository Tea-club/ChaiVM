find_package(FLEX REQUIRED)

set(LEXER_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LEXER_OUT ${LEXER_DIR}/lexer.cpp)
flex_target(scanner
        asmlex.l
        ${LEXER_OUT}
        DEFINES_FILE ${LEXER_DIR}/lexer.hpp
)

add_library(front_asm STATIC)
target_link_libraries(front_asm PRIVATE
        chai_include
)
target_sources(front_asm PRIVATE
        assembler.cpp
    ${FLEX_scanner_OUTPUTS}
)
target_include_directories(front_asm PUBLIC
        ${LEXER_DIR}
)
