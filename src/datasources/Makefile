HQLROOT = ../..
include $(HQLROOT)/Makefile.globals
SRC = ..

CPP = g++
INCLUDES = -I$(SRC)/postgres/include/ -I$(SRC)/rasdaman/include -I$(SRC) \
	-I$(RMANINSTALL)/include -I$(RMANSRC)/include
CPPFLAGS = -g $(INCLUDES)

OUTLIB = libdatasources.a
OBJ = $(shell ls *.cpp | sed 's/cpp/o/')


all: $(OUTLIB)

$(OUTLIB): $(OBJ)
	ar cru $(OUTLIB)  *.o
	ranlib $(OUTLIB)


$(OBJ): %.o: %.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@


clean:
	rm -f $(OBJ) $(OUTLIB)
