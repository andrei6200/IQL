SUBDIRS = datasources grammar querytree postgres rasdaman

bin_PROGRAMS = hql

hql_SOURCES = driver.cpp HqlMain.cpp logger.cpp
hql_LDFLAGS = -L$(RASDAMAN)/lib
hql_LDADD = -lrasodmg -lclientcomm -lrasodmg -lraslib -lcompression -lnetwork -lcrypto -lpqxx grammar/parser.o grammar/lexer.o postgres/libpg.a rasdaman/librman.a querytree/libquerytree.a datasources/libdatasources.a -lpqxx -lrasodmg -lclientcomm -lrasodmg -lraslib -lcompression -lnetwork -lcrypto



AM_CXXFLAGS = -w -I$(RASDAMAN)/include -DLINUX -DEARLY_TEMPLATE -DONCRPC -DDEBUG 
