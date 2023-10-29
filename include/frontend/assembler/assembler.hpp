#pragma once

#include <FlexLexer.h>

#include "frontend/assembler/asmlex.hpp"

int yyFlexLexer::yywrap() { return 1; }

namespace front::assembler {

int lex() {
    auto lexer = std::make_unique<AsmLex>();
    while (lexer->yylex() != 0) {
        lexer->print_current();
    }
}

}