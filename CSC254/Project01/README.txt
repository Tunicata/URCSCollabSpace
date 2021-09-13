This is a CSC254 Assignment 1:  Comparing Languages (Prime Partitions) developed by Liwei Jiang(ljiang14 / ljiang14@u.rochester.edu) and YuanChen Tao(ytao9/ ytao9@u.rochester.edu)

The Assignment 1.zip will contain the following files:

	README.txt					This document
	primepartition.adb			A primepartition caculator implimented by Ada
	primepartition.hs			A primepartition caculator implimented by Haskell
	primepartition.pl			A primepartition caculator implimented by Prolog
	primepartition.py			A primepartition caculator implimented by Python
	primepartition.rs			A primepartition caculator implimented by Rust

Except primepartition.pl, every implimentation have its main procedure which would read an input integer from the command line and print out the prime partitions of it.

For Prolog you should follow the following steps and by input ";" to get all outputs:

[ljiang14@cycle1 ~]$ swipl
Welcome to SWI-Prolog (threaded, 64 bits, version 8.2.3)
SWI-Prolog comes with ABSOLUTELY NO WARRANTY. This is free software.
Please run ?- license. for legal details.

For online help and background, visit https://www.swi-prolog.org
For built-in help, use ?- help(Topic). or ?- apropos(Word).

?- consult("primepartition.pl").
true.

?- primePartition(24, P).     
P = [19, 5] ;
P = [19, 3, 2] ;
P = [17, 7] ;
P = [17, 5, 2] ;
P = [13, 11] ;
false.
