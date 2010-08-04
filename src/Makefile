#########################################################
#							#
#   Makefile for the Hybrid Query Language Processor	#
#							#
#########################################################

HQLROOT = ..
include $(HQLROOT)/Makefile.globals
SRC=.


OBJ=grammar/lexer.o grammar/parser.o driver.o

LIB_PG=postgres/libparse.a
LIB_RMAN=rasdaman/librqlparse.a
LIB_QT=querytree/libquerytree.a
LIB_DS=datasources/libdatasources.a
LIBTARGET=$(LIB_PG) $(LIB_RMAN) $(LIB_QT) $(LIB_DS)


#########################
##### Rules Section ##### 

all: hql

$(LIB_PG):
	make -C postgres

$(LIB_RMAN):
	make -C rasdaman

$(LIB_QT):
	make -C querytree

$(LIB_DS):
	make -C datasources

### Driver actually builds the parser driver program
hql: grammar utils driver.o HqlMain.o querytree datasources
	$(CPP) $(CPPFLAGS) driver.o HqlMain.o		\
	utils/HqlTable.o				\
	$(RASQL_LIBS) $(SQL_LIBS)			\
	grammar/parser.o grammar/lexer.o		\
	$(LIBS)						\
	-o hql

grammar: lib/libparse.a lib/librqlparse.a
	make -C grammar

querytree: lib/libquerytree.a
	cp $(LIB_QT) $(HQLROOT)/lib/

utils: utils/HqlTable.o
	make -C utils

datasources : lib/libdatasources.a
	cp $(LIB_DS) $(HQLROOT)/lib/

lib/libparse.a: $(LIB_PG)
	cp $(LIB_PG) $(HQLROOT)/lib/

lib/librqlparse.a: $(LIB_RMAN)
	cp $(LIB_RMAN) $(HQLROOT)/lib/

lib/libquerytree.a: $(LIB_QT)
	make -C querytree

lib/libdatasources.a: $(LIB_DS)
	make -C datasources

driver.o: driver.cpp
	$(CPP) $(CPPFLAGS) $(LIBS) -c driver.cpp   -o driver.o

HqlMain.o: HqlMain.cpp HqlMain.hpp utils/HqlTable.hpp
	$(CPP) $(CPPFLAGS) $(LIBS) -c HqlMain.cpp   -o HqlMain.o

clean:
	make -C postgres clean
	make -C rasdaman clean
	make -C grammar clean
	make -C querytree clean
	make -C utils clean
	make -C datasources clean
	rm -rvf *~ *.o y.tab.* lex.yy.* parser.output parser.c parser.h lexer.c teststr driver hql program.log hql_*
