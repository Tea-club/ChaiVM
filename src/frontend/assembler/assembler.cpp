#include <FlexLexer.h>

#include "frontend/assembler/asmlex.hpp"

int yyFlexLexer::yywrap() { return 1; }

namespace front::assembler {

void lex() {
    auto lexer = std::make_unique<AsmLex>();
    while (lexer->yylex() != 0) {
        lexer->printCurrent();
    }
}

}