%% Autor: %AUTHOR%
%% Datum: %DATE%
:-use_module(utils).


pigeon(N,M):-
   NCons is N+M,
   Total is N * M,
   writeL(['file(pigeon-',M,'-',N,').']),
   writeL(['nbvar(',Total,').']),
   writeL(['nbconstr(',NCons,').']),
   writeL(['variableOrdering([]).']),
   numlist(1,Total,List),
   partition(N,List,Parts),
   forall(
          member(Z0,Parts),
          (
          makeParity(0,Z0,Z1),
          makeCoefs(1,Z1,Z2),
          append(Z2,[1],W),
          writeLine(W)
          )
   ),
   transpose(Parts,TParts),
   forall(
          member(Z0,TParts),
          (
          makeParity(1,Z0,Z1),
          makeCoefs(1,Z1,Z2),
          M1 is M - 1,
          append(Z2,[M1],W),
          writeLine(W)
          )
   ).

createPigeonHole2(N,M):-
   NCons is N+M,
   Total is N * M,
   write('* #variable= '),write(Total),
   write(' #constraint= '),write(NCons),nl,
   write('****************************************'),nl,
   write('* begin normalizer comments'),nl,
   write('* category= SAT/UNSAT-SMALLINT'),nl,
   write('* end normalizer comments'),nl,
   write('****************************************'),nl,
   Total is N * M,
   numlist(1,Total,List),
   partition(M,List,Parts),
   transpose(Parts,TParts),
   forall(
          member(Z1,Parts),
          (
          makeCoefs(-1,Z1,Z2),
          append(Z2,[-1],W),
          writeLine(W)
          )
   ),
   forall(
          member(Z1,TParts),
          (
          makeCoefs(1,Z1,Z2),
          append(Z2,[1],W),
          writeLine(W)
          )
   ).



createPigeonHole(N,M):-
   NCons is N+M,
   Total is N * M,
   write('* #variable= '),write(Total),
   write(' #constraint= '),write(NCons),nl,
   write('****************************************'),nl,
   write('* begin normalizer comments'),nl,
   write('* category= SAT/UNSAT-SMALLINT'),nl,
   write('* end normalizer comments'),nl,
   write('****************************************'),nl,
   Total is N * M,
   numlist(1,Total,List),
   partition(N,List,Parts),
   forall(
          member(Z1,Parts),
          (
          makeCoefs(1,Z1,Z2),
          append(Z2,[1],W),
          writeLine(W)
          )
   ),
   transpose(Parts,TParts),
   forall(
          member(Z1,TParts),
          (
          makeCoefs(-1,Z1,Z2),
          append(Z2,[-1],W),
          writeLine(W)
          )
   ).



transpose(M,N):-
   M = [X|_],
   length(X,L),
   transpose(0,L,M,N).

transpose(Now,Now,_,[]):- !.
transpose(Now,Final,M,[L|N]):-
   getNth0Column(Now,M,L),
   Now1 is Now + 1,
   transpose(Now1,Final,M,N).


getNth0Column(_,[],[]):-!.
getNth0Column(N,[C|X],[E|R]):-
   nth0(N,C,E),
   getNth0Column(N,X,R).


partition(_,[],[]):- !.
partition(N,X,[F|Y]):-
   firstN(N,X,F,Res),
   partition(N,Res,Y).

firstN(0,Res,[],Res):-!.
firstN(N,[A|X],[A|Y],Res):-
   N1 is N - 1,
   firstN(N1,X,Y,Res).


makeCoefs(_,[],[]):-!.
makeCoefs(C,[B|X],[[C|B]|Y]):-
   makeCoefs(C,X,Y).

makeParity(_,[],[]):-!.
makeParity(C,[B|X],[[B,C]|Y]):-
   makeParity(C,X,Y).

   
writeLine([[A,B,C]|D]):-
   write('constraint('),
   write('[['),
   write(B),
   write(','),
   write(A),
   write(','),
   write(C),
   write(']'),
   writeLine2(D).

writeLine2([A]):-
   !,
   write('],'),
   write(A),
   write(').'),
   nl.
   
writeLine2([[A,B,C]|X]):-
   write(',['),
   write(B),
   write(','),
   write(A),
   write(','),
   write(C),
   write(']'),
   writeLine2(X).
