# START: Taken from PostgreSQL Makefile
CC = colorgcc
PGCFLAGS = -g -O2 -Wall -Wmissing-prototypes -Wpointer-arith -Wdeclaration-after-statement -Wendif-labels -fno-strict-aliasing -fwrapv 
# END: Taken from PostgreSQL Makefile


FLEX = flex
YACC = bison
VALGRIND = valgrind

INCLUDES = -Ipostgres/include/ 
LIBS = -Lpostgres/lib/ -lparse
CFLAGS = $(PGCFLAGS) $(INCLUDES) -D_GNU_SOURCE
FLEXFLAGS = -CF
YACCFLAGS = -d --report=itemset
# YACCFLAGS = --verbose -d

all: subdir
	$(FLEX) $(FLEXFLAGS) -o "lexer.c" lexer.ll
	$(YACC) $(YACCFLAGS) parser.yy  -o parser.c
	$(CC) $(CFLAGS) lexer.c parser.c $(LIBS) -o parser  

subdir:
	make -C postgres

check:
	$(CC) -g teststr.c str.c -o teststr
	$(VALGRIND) --leak-check=full ./teststr

clean:
	rm -f *~ *.o y.tab.* lex.yy.* parser *.hpp *.cpp parser.output parser.c parser.h lexer.c teststr
	make -C postgres clean
