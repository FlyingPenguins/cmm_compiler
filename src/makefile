# Makefile for cmm compiler

all:	cmm

cmm:	bison flex
	bison -d cmm.y && \
	flex -ocmm.lex.c cmm.l && \
	cc -g -o $@ cmm.tab.c cmm.tab.h cmm.lex.c symtable.c symtable.h -lm

bison:	cmm.y
	bison -d cmm.y && \
	cc -g -o $@ cmm.tab.c cmm.tab.h -lm

flex:	cmm.l
	flex -ocmm.lex.c cmm.l && \
	cc -g -o $@ cmm.lex.c -lm

clean:
	rm -f cmm.tab.c cmm.tab.h \
	cmm.lex.c *.o *~
