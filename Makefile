all: clean
	flex -o "lexer.c" lexer.ll
	yacc --verbose -d parser.yy  -o parser.c
	gcc lexer.c parser.c -o parser

clean:
	rm -f *~ *.o y.tab.* lex.yy.* parser *.hpp *.cpp parser.output parser.c parser.h lexer.c
