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

all: HqlTable.o

HqlTable.o: HqlTable.cpp HqlTable.hpp
	$(CPP) $(CPPFLAGS) $(LIBS) -c HqlTable.cpp  -o HqlTable.o

clean:
	rm -rvf *~ *.o
