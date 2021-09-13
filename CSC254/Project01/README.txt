CSC254 Assignment 1 :\
YuanChen Tao(ytao9/ ytao9@u.rochester.edu) and
Liwei Jiang(ljiang14 / ljiang14@u.rochester.edu)

The Assignment_1.zip contain the following files:

README.txt				This document
primepartition.adb			Ada implementation
primepartition.hs			Haskell implementation
primepartition.pl			Prolog implementation
primepartition.py			Python implementation
primepartition.rs			Rust implementation

Only prolog implementation don't have main implementation, all other languages have their main implementation to read inputs from Scanner. 

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
