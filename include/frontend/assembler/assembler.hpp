#pragma once

#include <FlexLexer.h>

#include "frontend/assembler/asmlex.hpp"

int yyFlexLexer::yywrap() { return 1; }

namespace front::assembler {

}