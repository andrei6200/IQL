#-------------------------------------------------------------------------
#
# Makefile for parser
#
# $PostgreSQL: pgsql/src/backend/parser/Makefile,v 1.49 2009/03/07 00:13:57 alvherre Exp $
#
#-------------------------------------------------------------------------

subdir = postgres
top_builddir = ..

# ************************* AA: START - We need the following to be able to use this Makefile outside the PG sourcetree
srcdir = .
CC = g++
OUTLIB = libparse.a

# The following is taken from Postgresql Makefile
CFLAGS = -Iinclude/ -O2 -Wall -Wpointer-arith -Wendif-labels -fno-strict-aliasing -fwrapv -g
# ************************* AA: END - We need the following to be able to use this Makefile outside the PG sourcetree


OBJS= keywords.o kwlookup.o

FLEXFLAGS = -CF

# include $(top_srcdir)/src/backend/common.mk


# ********************* AA: START - New targets

# all is the default target
all: $(OUTLIB)

$(OUTLIB): $(OBJS)
	ar cru $(OUTLIB)  *.o
	ranlib $(OUTLIB)


$(OBJS):  %.o: %.c include/parser/kwlist.h
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm -f $(OBJS) $(OUTLIB)

# ********************* AA: END - New targets

