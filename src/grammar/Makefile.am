#########################################################
#							#
#   Makefile for the Hybrid Query Language Processor	#
#							#
#########################################################

FLEXFLAGS = -CF
YACCFLAGS = -d

CXXFLAGS += -w -I.. -I$(RASDAMAN)/include -I../postgres/include -I../rasdaman/include


#########################
##### Rules Section ##### 

all: parser.o lexer.o

parser.o: parser.cpp parser.hpp str.c str.h 
	$(CXX) $(CXXFLAGS) -c parser.cpp   -o parser.o

lexer.o: lexer.cpp
	$(CXX) $(CXXFLAGS) -c lexer.cpp   -o lexer.o

### Generate the parser/lexer sources with Flex/Bison
parser.cpp parser.hpp: parser.yy
	$(YACC) $(YACCFLAGS) parser.yy   -o parser.cpp

lexer.cpp: lexer.ll
	$(LEX) $(FLEXFLAGS) -o "lexer.cpp" lexer.ll


clean:
	rm -rvf *~ *.o y.tab.* lex.yy.* parser.output parser.cpp parser.hpp lexer.cpp teststr driver hql
