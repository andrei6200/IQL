HQLROOT = ../..
SRC = ..
include $(HQLROOT)/Makefile.globals


OUTLIB = librqlparse.a

OBJS= rqlkeywords.o rqlkwlookup.o

# all is the default target
all: $(OUTLIB)

$(OUTLIB): $(OBJS)
	ar cru $(OUTLIB)  *.o
	ranlib $(OUTLIB)


$(OBJS):  %.o: %.c include/rqlkwlist.h $(SRC)/grammar/parser.h
	$(CPP) -c $(CPPFLAGS) $< -o $@


$(SRC)/grammar/parser.h:
	make -C $(SRC)/grammar

clean:
	rm -f $(OBJS) $(OUTLIB)
