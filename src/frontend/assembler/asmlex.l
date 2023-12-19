%option yyclass="front::assembler::AsmLex"
%option c++

%{

#include "frontend/assembler/asmlex.hpp"

%}

WS      [ \n\t\v]+
COMMENT #.*

INT             [+-]?[1-9][0-9]*
FLOAT           [+-]?([1-9][0-9]*\.?[0-9]*|0\.[0-9]*|0)
IDENTIFIER      [a-zA-Z_][a-zA-Z0-9_.]*
FUNC            "fn"
STRING          \"(\\.|[^"\\])*\"
COMMA           ,

%%

{WS}                /* skip */
{COMMENT}           /* skip */
{INT}               return processInt();
{FLOAT}             return processFloat();
{IDENTIFIER}        return processIdentifier();
"fn"                return processFunc();
"{"                 return processOpCurlyBracket();
"}"                 return processClCurlyBracket();
{COMMA}             return processComma();
{STRING}            return processString();
.                   return processUnknown();

%%

// nothing