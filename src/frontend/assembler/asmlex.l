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
FLOAT   [+-]?(

COMMA           ,
COLON           :

IDENTIFIER      [a-zA-Z_][a-zA-Z0-9_.]*

%%

{WHITE_SPACE}       /* Skip */
{COMMENT}           /* Skip */

{COMMA}             return processComma();

{NUMBER}            return processNumber();
{IDENTIFIER}{COLON} return processLabel();
{IDENTIFIER}        return processIdentifier();

.                   return processUndef();

WS      [ \t\n\v]+
DIGIT   [0-9]
DIGIT1  [1-9]
LETTER  [a-zA-Z]

%%

{WS}              /* skip blanks and tabs */
{DIGIT1}{DIGIT}*  return processDigit();
{LETTER}+         return processInstruction();
.                 return processUnknown();

%%

// nothing