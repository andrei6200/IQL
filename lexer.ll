%{
// #define YYSTYPE char *

#include <stdio.h>
#include "parser.h"

%}

%option noyywrap

%%




test					return TEST;
select					return SELECT;
[0-9]+					yylval = atoi(yytext); return NUMBER;
;                       return SEMICOLON;
\n                      return EOL;
[ \t]+                  /* ignore whitespace */;
%%
