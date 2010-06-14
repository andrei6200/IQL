SRC = ..

CC = gcc
OUTLIB = librqlparse.a

CFLAGS = -Iinclude/ -I$(SRC) -O2 -Wall -Wmissing-prototypes -Wpointer-arith -Wdeclaration-after-statement -Wendif-labels -fno-strict-aliasing -fwrapv -g

OBJS= rqlkeywords.o rqlkwlookup.o

# all is the default target
all: $(OUTLIB)

$(OUTLIB): $(OBJS)
	ar cru $(OUTLIB)  *.o
	ranlib $(OUTLIB)


$(OBJS):  %.o: %.c include/rqlkwlist.h $(SRC)/grammar/parser.h
	$(CC) -c $(CFLAGS) $< -o $@


$(SRC)/grammar/parser.h:
	make -C $(SRC)/grammar

clean:
	rm -f $(OBJS) $(OUTLIB)
