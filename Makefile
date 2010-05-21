all: clean
	lex lexer.ll
	yacc --verbose -d parser.yy  -o parser.cpp
	gcc -c lex.yy.c -o lex.yy.o
	g++ lex.yy.o parser.cpp -o parser

clean:
	rm -f *~ *.o y.tab.* lex.yy.* parser *.hpp *.cpp parser.output parser.c parser.h
