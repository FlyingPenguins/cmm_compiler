# $Header: /home/johnl/flnb/code/RCS/Makefile.ch3,v 2.1 2009/11/08 02:53:18 johnl Exp $
# Companion source code for "flex & bison", published by O'Reilly
# Media, ISBN 978-0-596-15597-1
# Copyright (c) 2009, Taughannock Networks. All rights reserved.
# See the README file for license conditions and contact info.

# programs in chapter 3

all:	cmm

cmm:	cmm.l cmm.y
	bison -d cmm.y && \
        flex -ocmm.lex.c cmm.l && \
        cc -g -o $@ cmm.tab.c cmm.tab.h cmm.lex.c -lm

bison:	cmm.y
	bison -d cmm.y && \
	cc -g -o $@ cmm.tab.c cmm.tab.h -lm

flex:	cmm.l
	flex -ocmm.lex.c cmm.l && \
	cc -g -o $@ cmm.lex.c -lm

clean:
	rm -f cmm.tab.c cmm.tab.h \
	cmm.lex.c *.o
