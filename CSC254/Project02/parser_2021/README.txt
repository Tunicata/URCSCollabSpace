This is a CSC254 Assignment 2:  Syntax Error Recovery developed by Liwei Jiang(ljiang14 / ljiang14@u.rochester.edu) and YuanChen Tao(ytao9/ ytao9@u.rochester.edu)

The Assignment 2.zip will contain the following files:

	README.txt		This document
	scan.h			A C++ .h head file include the token enum and imprted packages.
	scan.cpp		A C++ .cpp file include an implementation of a token scanner could recover from lexical errors.
	parse.cpp		A C++ .cpp file include an implementation of a parser take an extanded calculator language as input, parse it, and output an linear, parenthesized form tree, and could recover from syntax errors.
	Makefile		Makefile
	test			test input for the parser which is same as the sample on assignment web page.

you could use the make instruction to quickly compile this project. After run it with a file as input, you would see a linear, parenthesized form tree in a successful case,. While for cases with errors, you would also get a syntax tree, but it may not same as the user expect and contain incomplete parts. You could trace back the syntax error, and unexpected token message in the outputs before the parse tree.