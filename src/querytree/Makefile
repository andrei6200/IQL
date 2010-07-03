HQLROOT = ../..
include $(HQLROOT)/Makefile.globals
SRC = ..

##### Home of Rasdaman installation #####
RMANINSTALL = /var/lib/postgresql/installed/rasdaman.community
RMANSRC = /var/lib/postgresql/rasdaman.community

CPP = g++
INCLUDES = -I$(SRC)/postgres/include/ -I$(SRC)/rasdaman/include -I$(SRC) \
	-I$(RMANINSTALL)/include -I$(RMANSRC)/include
CPPFLAGS = -g $(INCLUDES)

OUTLIB = libquerytree.a
OBJ = QtNode.o QtSource.o QtReference.o QtList.o QtSelect.o QtDot.o QtString.o


all: $(OUTLIB)

$(OUTLIB): $(OBJ)
	ar cru $(OUTLIB)  *.o
	ranlib $(OUTLIB)


$(OBJ): %.o: %.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@


clean:
	rm -f $(OBJ) $(OUTLIB)
