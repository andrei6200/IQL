#########################################################
#							#
#   Makefile for the Hybrid Query Language Processor	#
#							#
#########################################################


#######################################
##### Various applications needed #####
CC = gcc
CPP = g++
FLEX = flex
YACC = bison
VALGRIND = valgrind

##### Home of Rasdaman installation #####
RMANHOME = /var/lib/postgresql/installed/rasdaman.community

##### Compilation flags for PostgreSQL, and Rasdaman respectively #####
PGCFLAGS= -O2 -Wall -Wpointer-arith -Wendif-labels -fno-strict-aliasing -fwrapv -D_GNU_SOURCE
RMANCFLAGS = -DLINUX -DEARLY_TEMPLATE -DONCRPC -DDEBUG

INCLUDES = -I./postgres/include/  -I$(RMANHOME)/include -I$(RMANHOME)/../../rasdaman.community/ -I./rasdaman/1hqlparser/include
CFLAGS = $(PGCFLAGS) $(RMANCFLAGS) $(INCLUDES) -g
CPPFLAGS = $(RMANCFLAGS)  $(INCLUDES) -g


#############################
##### Libraries section #####

# SQL Grammar and RaSQL grammar libraries
GRAMMAR_LIBS = -Lpostgres/lib/ -Lrasdaman/1hqlparser/lib/ -lparse -lrqlparse
# PostgreSQL client support
SQL_LIBS = -lpqxx
# Rasdaman client support
RASQL_LIBS = -L/usr/lib -L$(RMANHOME)/lib					\
	-lrasodmg -lclientcomm -lrasodmg -lraslib -lcompression -lnetwork -lcrypto 

LIBS = $(GRAMMAR_LIBS) $(SQL_LIBS) $(RASQL_LIBS)

FLEXFLAGS = -CF
YACCFLAGS = -d


OBJ=lexer.o parser.o driver.o
LIB_PG=postgres/lib/libparse.a
LIB_RMAN=rasdaman/1hqlparser/lib/librqlparse.a
LIBTARGET=$(LIB_PG) $(LIB_RMAN)


#########################
##### Rules Section ##### 

all: hql

$(LIB_PG):
	make -C postgres

$(LIB_RMAN):
	make -C rasdaman/1hqlparser

### Driver actually builds the parser driver program
hql: parser.o lexer.o driver.o HqlMain.o $(LIBTARGET)
	$(CPP) $(CPPFLAGS) driver.o HqlMain.o $(RASQL_LIBS) $(SQL_LIBS) parser.o lexer.o \
		$(GRAMMAR_LIBS)  -o hql

driver.o: driver.cpp
	$(CPP) $(CPPFLAGS) $(LIBS) -c driver.cpp   -o driver.o

HqlMain.o: HqlMain.cpp HqlMain.hpp
	$(CPP) $(CPPFLAGS) $(LIBS) -c HqlMain.cpp   -o HqlMain.o

parser.o: parser.c parser.h str.c str.h
	$(CPP) $(CFLAGS) $(LIBS) -c parser.c   -o parser.o

lexer.o: lexer.c
	$(CC) $(CFLAGS) $(LIBS) -c lexer.c   -o lexer.o

### Generate the parser/lexer sources with Flex/Bison
parser.c parser.h: parser.yy
	$(YACC) $(YACCFLAGS) parser.yy   -o parser.c

lexer.c: lexer.ll
	$(FLEX) $(FLEXFLAGS) -o "lexer.c" lexer.ll


### Tests
check:
	$(CC) -g teststr.c str.c -o teststr
	$(VALGRIND) --leak-check=full ./teststr

clean:
	rm -f *~ *.o y.tab.* lex.yy.* parser.output parser.c parser.h lexer.c teststr driver hql
	make -C postgres clean
