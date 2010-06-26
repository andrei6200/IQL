#########################################################
#							#
#   Makefile for the Hybrid Query Language Processor	#
#							#
#########################################################

HQLROOT = ../..
include $(HQLROOT)/Makefile.globals
SRC = ..

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
