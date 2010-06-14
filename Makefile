#########################################################
#							#
#   Makefile for the Hybrid Query Language Processor	#
#							#
#########################################################

# This file just dispatches incoming requests to the Makefile in the src/ folder.


all: hql
	
hql: 
	make -C src

clean:
	rm -f lib/*.a
	make -C src clean
