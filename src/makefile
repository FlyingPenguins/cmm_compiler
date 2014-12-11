# Makefile for cmm compiler

all:	cmm

cmm:	cmm.y cmm.l cmm.h cmmfuncs.c
	bison -d cmm.y && \
	flex -ocmm.lex.c cmm.l && \
	gcc -g -o $@ cmm.tab.c cmm.tab.h \
	cmm.lex.c cmm.h cmmfuncs.c -lm

bison:	cmm.y cmm.h cmmfuncs.c
	bison -d cmm.y && \
	gcc -g -o $@ cmm.tab.c cmm.tab.h cmm.h cmmfuncs.c -lm

flex:	cmm.l cmm.h cmm.tab.h
	flex -ocmm.lex.c cmm.l && \
	gcc -g -o $@ cmm.lex.c cmm.h cmm.tab.h -lm

funcs:	cmm.h cmmfuncs.c
	gcc -g -o $@ cmm.h cmmfuncs.c

clean:
	rm -f cmm.tab.c cmm.tab.h \
	cmm.lex.c *.o *~ bison flex funcs cmm \
	cmm.output
