%option yyclass="front::assembler::AsmLex"
%option c++

%{

#include "frontend/assembler/asmlex.hpp"

%}

WS      [ \n\t\v]+
COMMENT #.*

DIGIT   [0-9]
DIGIT1  [1-9]
NUMBER  {DIGIT1}{DIGIT}*
INT     [+-]?({DIGIT1}{DIGIT}*\.?[0-9]*|0\.[0-9]*)
IDENTIFIER      [a-zA-Z_][a-zA-Z0-9_.]*

COMMA           ,
COLON           :

%%

{WS}              /* skip blanks and tabs */
{DIGIT1}{DIGIT}*  return processDigit();
IDENTIFIER        return processIdentifier();
.                 return processUnknown();

%%

// nothing