# Makefile for cmm compiler

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
	cmm.lex.c *.o *~
