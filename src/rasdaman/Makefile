SRC = ..

CC = gcc
CPP = g++
OUTLIB = librqlparse.a

CFLAGS = -Iinclude/ -I$(SRC) -O2 -Wall -Wpointer-arith -fno-strict-aliasing -fwrapv -g

OBJS= rqlkeywords.o rqlkwlookup.o

# all is the default target
all: $(OUTLIB)

$(OUTLIB): $(OBJS)
	ar cru $(OUTLIB)  *.o
	ranlib $(OUTLIB)


$(OBJS):  %.o: %.c include/rqlkwlist.h $(SRC)/grammar/parser.h
	$(CPP) -c $(CFLAGS) $< -o $@


$(SRC)/grammar/parser.h:
	make -C $(SRC)/grammar

clean:
	rm -f $(OBJS) $(OUTLIB)
