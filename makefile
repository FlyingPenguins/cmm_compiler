# $Header: /home/johnl/flnb/code/RCS/Makefile.ch3,v 2.1 2009/11/08 02:53:18 johnl Exp $
# Companion source code for "flex & bison", published by O'Reilly
# Media, ISBN 978-0-596-15597-1
# Copyright (c) 2009, Taughannock Networks. All rights reserved.
# See the README file for license conditions and contact info.

# programs in chapter 3

all:	cmm

bison:	cmm_parser.y
	bison -d cmm_parser.y
	cc -o $@ cmm_parser.tab.c cmm_parser.tab.h

cmm:	fb3-2.l fb3-2.y fb3-2.h fb3-2funcs.c
	bison -d fb3-2.y && \
	flex -ofb3-2.lex.c fb3-2.l && \
	cc -g -o $@ fb3-2.tab.c fb3-2.lex.c fb3-2funcs.c -lm

clean:
	rm -f cmm_parser.tab.c cmm_parser.tab.h
