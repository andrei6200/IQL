%{
#include <stdio.h>
#include <string.h>

extern int yylex();

%}

/******************************** START POSTGRESQL *********************************/
/* Special token types, not actually keywords - see the "lex" file */
%token <str>    IDENT FCONST SCONST BCONST XCONST Op 
%token <ival>   ICONST PARAM
%left 			TYPECAST
/******************************** END POSTGRESQL *********************************/

%token <str> HQLNUMBER HQLSELECT HQLTEST HQLSEMICOLON HQLEOL
%error-verbose
%locations

%union
{
  char		*str;
  char		*keyword;
  int		ival;
}

%%

commands:
        |        
		  commands command HQLEOL
		| 
		  commands command HQLSEMICOLON
        ;

command:	
		HQLTEST		
			{ printf("Statement: TEST\n"); }
		| HQLTEST HQLNUMBER
			{ printf("Statement: TEST number: %s\n", $2); }
		| HQLSELECT HQLNUMBER
			{ printf("Statement: SELECT number: %s\n", $2); }

%%


int main()
{
	yyparse();
	return 0;
} 
