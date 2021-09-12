isPrime(2).
isPrime(Num) :-
    Num > 2,
    !,
    NewNum is ceiling(sqrt(Num)),
    isDivisible(Num, NewNum).

isDivisible(Num, X) :-
    X > 1,
    Num mod X =\= 0 ,
    isDivisible(Num, X-1).
isDivisible(_, X) :-
    1 is X.

primeGenerator(Num, []) :- Num < 2, !.
primeGenerator(Num, [Num|Lst]) :-
    isPrime(Num),
    !,
    Next is Num - 1,
    primeGenerator(Next, Lst).
primeGenerator(Num, Lst) :-
    Next is Num - 1,
    primeGenerator(Next, Lst).

subset([], []).
subset([E|T], [E|NT]):-
  subset(T, NT).
subset([_|T], NT):-
  subset(T, NT).

sumList([], 0).
sumList([H|T], Sum) :-
   sumList(T, Rest),
   Sum is H + Rest.

primePartition(Num, P) :-
    primeGenerator(Num, X),
    subset(X, P),
    sumList(P, Num).