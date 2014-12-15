cmm_compiler
============

Term project for compiler theory class using flex and bison to create a compiler for a subset of C++ A.K.A. C--

Make targets:

* `all`: compiles bison, flex and any additional utility files. Output is `cmm` executable file.

* `clean`: removes all compiled/executable files

After building, the code can currently be tested by running:

* `./cmm < ../tests/example.c`
