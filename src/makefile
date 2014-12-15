# Makefile for cmm compiler

all:	cmm

cmm:	cmm.y cmm.l cmm.h cmmfuncs.c
	bison -d cmm.y && \
	flex -ocmm.lex.c cmm.l && \
	gcc -g -o $@ cmm.tab.c cmm.tab.h \
	cmm.lex.c cmm.h cmmfuncs.c -lm

clean:
	rm -f cmm.tab.c cmm.tab.h \
	cmm.lex.c *.o *~ bison flex funcs cmm \
	cmm.output
