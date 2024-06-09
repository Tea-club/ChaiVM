%option yyclass="front::assembler::AsmLex"
%option c++

%{

#include "frontend/assembler/asmlex.hpp"

%}

WS      [ \n\t\v]+
COMMENT #.*

INT             [+-]?0|([1-9][0-9]*)
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
"fn"                return processFunc();
"{"                 return processOpCurlyBracket();
"}"                 return processClCurlyBracket();
{COMMA}             return processComma();
{IDENTIFIER}        return processIdentifier();
{STRING}            return processString();
<<EOF>>             return processEOF();
.                   return processUnknown();

%%

// nothing