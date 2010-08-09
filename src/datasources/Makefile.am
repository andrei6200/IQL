noinst_LIBRARIES = libdatasources.a
libdatasources_a_SOURCES = DataSourceInterface.cpp  PostgresDS.cpp  RasdamanDS.cpp
AM_CXXFLAGS = -I.. -I@RASDAMAN@/include -DLINUX -DEARLY_TEMPLATE -DONCRPC -DDEBUG
AM_LDFLAGS = -L$(RASDAMAN)/lib 
LDADD = -lrasodmg -lclientcomm -lrasodmg -lraslib -lcompression -lnetwork -lcrypto -lpqxx

bin_PROGRAMS = test
test_SOURCES = test.cpp ../logger.cpp ../HqlTable.cpp RasdamanDS.cpp DataSourceInterface.cpp
#test_DEPENDENCIES = libdatasources.a
#test_LDADD = $(LDADD) libdatasources.a 
