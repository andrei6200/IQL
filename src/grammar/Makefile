#########################################################
#							#
#   Makefile for the Hybrid Query Language Processor	#
#							#
#########################################################

HQLROOT = ../..
SRC = ..

#######################################
##### Various applications needed #####
CC = gcc
CPP = g++
FLEX = flex
YACC = bison
VALGRIND = valgrind

##### Home of Rasdaman installation #####
RMANINSTALL = /var/lib/postgresql/installed/rasdaman.community
RMANSRC = /var/lib/postgresql/rasdaman.community

##### Compilation flags for PostgreSQL, and Rasdaman respectively #####
PGCFLAGS= -O2 -Wall -Wpointer-arith -Wendif-labels -fno-strict-aliasing -fwrapv -D_GNU_SOURCE
RMANCFLAGS = -DLINUX -DEARLY_TEMPLATE -DONCRPC -DDEBUG

INCLUDES = -I$(SRC)/postgres/include/ -I$(SRC)/rasdaman/include -I$(SRC) \
	-I$(RMANINSTALL)/include -I$(RMANSRC)/include
CFLAGS = $(PGCFLAGS) $(RMANCFLAGS) $(INCLUDES) -g
CPPFLAGS = $(RMANCFLAGS)  $(INCLUDES) -g


#############################
##### Libraries section #####

# SQL Grammar and RaSQL grammar libraries
GRAMMAR_LIBS = -L$(HQLROOT)/lib -lparse -lrqlparse
# PostgreSQL client support
SQL_LIBS = -lpqxx
# Rasdaman client support
RASQL_LIBS = -L/usr/lib -L$(RMANHOME)/lib					\
	-lrasodmg -lclientcomm -lrasodmg -lraslib -lcompression -lnetwork -lcrypto

LIBS = $(GRAMMAR_LIBS) $(SQL_LIBS) $(RASQL_LIBS)

FLEXFLAGS = -CF
YACCFLAGS = -d


OBJ=grammar/lexer.o grammar/parser.o driver.o
LIB_PG=$(SRC)/postgres/libparse.a
LIB_RMAN=$(SRC)rasdaman/librqlparse.a
LIBTARGET=$(LIB_PG) $(LIB_RMAN)


#########################
##### Rules Section ##### 

all: parser.o lexer.o

parser.o: parser.c parser.h str.c str.h
	$(CPP) $(CPPFLAGS) $(LIBS) -c parser.c   -o parser.o

lexer.o: lexer.c
	$(CC) $(CFLAGS) $(LIBS) -c lexer.c   -o lexer.o

### Generate the parser/lexer sources with Flex/Bison
parser.c parser.h: parser.yy
	$(YACC) $(YACCFLAGS) parser.yy   -o parser.c

lexer.c: lexer.ll
	$(FLEX) $(FLEXFLAGS) -o "lexer.c" lexer.ll


clean:
	rm -rvf *~ *.o y.tab.* lex.yy.* parser.output parser.c parser.h lexer.c teststr driver hql
