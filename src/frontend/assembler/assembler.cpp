#include <FlexLexer.h>

#include "frontend/assembler/assembler.hpp"

int yyFlexLexer::yywrap() { return 1; }

namespace front::assembler {}