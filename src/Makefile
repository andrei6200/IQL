#########################################################
#							#
#   Makefile for the Hybrid Query Language Processor	#
#							#
#########################################################

HQLROOT = ..

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

INCLUDES = -I./postgres/include/ -I./rasdaman/include -I$(RMANINSTALL)/include -I$(RMANSRC)
CFLAGS = $(PGCFLAGS) $(RMANCFLAGS) $(INCLUDES) -g
CPPFLAGS = $(RMANCFLAGS)  $(INCLUDES) -g


#############################
##### Libraries section #####

# SQL Grammar and RaSQL grammar libraries
GRAMMAR_LIBS = -L$(HQLROOT)/lib -lparse -lrqlparse
# PostgreSQL client support
SQL_LIBS = -lpqxx
# Rasdaman client support
RASQL_LIBS = -L/usr/lib -L$(RMANINSTALL)/lib					\
	-lrasodmg -lclientcomm -lrasodmg -lraslib -lcompression -lnetwork -lcrypto 


LIBS = $(GRAMMAR_LIBS) $(SQL_LIBS) $(RASQL_LIBS)

FLEXFLAGS = -CF
YACCFLAGS = -d


OBJ=grammar/lexer.o grammar/parser.o driver.o
LIB_PG=postgres/libparse.a
LIB_RMAN=rasdaman/librqlparse.a
LIBTARGET=$(LIB_PG) $(LIB_RMAN)


#########################
##### Rules Section ##### 

all: hql

$(LIB_PG):
	make -C postgres

$(LIB_RMAN):
	make -C rasdaman

### Driver actually builds the parser driver program
hql: grammar driver.o HqlMain.o logger.o
	$(CPP) $(CPPFLAGS) driver.o HqlMain.o logger.o $(RASQL_LIBS) $(SQL_LIBS)  \
	grammar/parser.o grammar/lexer.o $(GRAMMAR_LIBS) -o hql

grammar: lib/libparse.a lib/librqlparse.a
	make -C grammar

lib/libparse.a: $(LIB_PG)
	cp $(LIB_PG) $(HQLROOT)/lib/

lib/librqlparse.a: $(LIB_RMAN)
	cp $(LIB_RMAN) $(HQLROOT)/lib/

driver.o: driver.cpp logger.hpp
	$(CPP) $(CPPFLAGS) $(LIBS) -c driver.cpp   -o driver.o

logger.o: logger.cpp logger.hpp
	$(CPP) $(CPPFLAGS) $(LIBS) -c logger.cpp -o logger.o

HqlMain.o: HqlMain.cpp HqlMain.hpp logger.hpp
	$(CPP) $(CPPFLAGS) $(LIBS) -c HqlMain.cpp   -o HqlMain.o

### Tests
clean:
	make -C postgres clean
	make -C rasdaman clean
	make -C grammar clean
	rm -rvf *~ *.o y.tab.* lex.yy.* parser.output parser.c parser.h lexer.c teststr driver hql
