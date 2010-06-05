CC = colorgcc
OUTLIB = lib/librqlparse.a

CFLAGS = -Iinclude/ -I../../ -O2 -Wall -Wmissing-prototypes -Wpointer-arith -Wdeclaration-after-statement -Wendif-labels -fno-strict-aliasing -fwrapv

OBJS= rqlkeywords.o rqlkwlookup.o

# all is the default target
all: $(OUTLIB)

$(OUTLIB): $(OBJS)
	ar cru $(OUTLIB)  *.o
	ranlib $(OUTLIB)


$(OBJS):  %.o: %.c include/rqlkwlist.h ../../parser.h
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -f $(OBJS) $(OUTLIB)
