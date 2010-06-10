# START: Taken from PostgreSQL Makefile
CC = gcc
CPP = g++
PGCFLAGS= -g -O2 -Wall -Wpointer-arith -Wendif-labels -fno-strict-aliasing -fwrapv
# END: Taken from PostgreSQL Makefile

FLEX = flex
YACC = bison
VALGRIND = valgrind

INCLUDES = -Ipostgres/include/  -Irasdaman/
LIBS = -Lpostgres/lib/ -Lrasdaman/rasql/lib/ -lparse -lrqlparse
CFLAGS = $(PGCFLAGS) $(INCLUDES) -D_GNU_SOURCE
FLEXFLAGS = -CF
YACCFLAGS = -d --report=itemset
# YACCFLAGS = --verbose -d

OBJ=lexer.o parser.o driver.o
LIB=postgres/lib/libparse.a rasdaman/rasql/lib/librqlparse.a

all: driver

postgres/lib/libparse.a:
	make -C postgres

rasdaman/rasql/lib/librqlparse.a:
	make -C rasdaman/rasql

### Driver actually builds the parser driver program
driver: parser.o lexer.o driver.o $(LIB)
	$(CPP) driver.o parser.o lexer.o $(LIBS) -o driver

parser.o: parser.c parser.h str.c str.h
	$(CC) $(CFLAGS) parser.c $(LIBS) -c -o parser.o

lexer.o: lexer.c
	$(CC) $(CFLAGS) lexer.c $(LIBS) -c -o lexer.o

driver.o: driver.cpp
	$(CPP) $(CPPFLAGS) driver.cpp $(LIBS) -c -o driver.o

### Generate the parser/lexer sources with Flex/Bison
parser.c parser.h: parser.yy
	$(YACC) $(YACCFLAGS) parser.yy  -o parser.c

lexer.c: lexer.ll
	$(FLEX) $(FLEXFLAGS) -o "lexer.c" lexer.ll


### Tests
check:
	$(CC) -g teststr.c str.c -o teststr
	$(VALGRIND) --leak-check=full ./teststr

clean:
	rm -f *~ *.o y.tab.* lex.yy.* *.hpp parser.output parser.c parser.h lexer.c teststr driver
	make -C postgres clean
