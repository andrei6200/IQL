noinst_LIBRARIES = libdatasources.a
libdatasources_a_SOURCES = DataSourceInterface.cpp  PostgresDS.cpp  RasdamanDS.cpp HqlTable.cpp

bin_PROGRAMS = test
test_SOURCES = test.cpp ../logger.cpp RasdamanDS.cpp DataSourceInterface.cpp HqlTable.cpp  PostgresDS.cpp

AM_CXXFLAGS = -w -I.. -I@RASDAMAN@/include -DLINUX -DEARLY_TEMPLATE -DONCRPC -DDEBUG
LDADD = -lrasodmg -lclientcomm -lrasodmg -lraslib -lcompression -lnetwork -lcrypto -lpqxx
AM_LDFLAGS = -L$(RASDAMAN)/lib 
