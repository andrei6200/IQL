%{
#include <stdio.h>
#include <string.h>

// #define YYSTYPE char *
 
void yyerror(const char *str)
{
        fprintf(stderr,"error: %s\n",str);
}
 
extern int yylex();

%}

%token NUMBER SELECT TEST SEMICOLON EOL
%error-verbose

%%

commands:
        |        
		  commands command EOL
		| 
		  commands command SEMICOLON
        ;

command:	
		TEST		
			{ printf("Statement: TEST\n"); }
		| TEST NUMBER
			{ printf("Statement: TEST number: %d\n", $2); }
		| SELECT NUMBER
			{ printf("Statement: SELECT number: %d\n", $2); }

%%


main()
{
        yyparse();
} 
