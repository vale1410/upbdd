:- module(utils,[shuffle/2,writeLTable/1,remove/3,writeMaxList/2,writelnList/1,writeTabLNum/1,doNtimes/2,writeTabL/1,writeTabLRound/1,applyList/2,aMember/3,choose2/2,chooseNth/3,sumSecondMinus/2,dot/3,writeL/1,extractNth/3]).

% doNtimes, performes a goal n times

doNtimes(0,_).
doNtimes(N,G):-
        call(G),
        N1 is N-1,
        doNtimes(N1,G).



% applyList(Pre,L) executes on all elements of List Pre, succeeds if all succeed.

applyList(_,[]):- !.
applyList(P,[H|T]) :-
        Goal =.. [P,H],
        Goal,
        applyList(P,T).

        
% Advanced Member, returns also the rest of the list.
% backtracking over the elements in the list.

aMember(X,[X|Y],Y).
aMember(X,[_|Y],Z) :- aMember(X,Y,Z).


%compares second arguments of a list.

choose2([_,A],[_,B]) :- A =< B.

chooseNth(N,AL,BL) :- 
        nth0(N,AL,A),
        nth0(N,BL,B),
        A =< B.
% sums up the second arguments of a list and times -1.

sumSecondMinus(L,S) :- 
        extractNth(L,1,N),
        sumlist(N,S1),
        S is -S1.

% extractNth(L,N,L2)
% extracts all nth argument of a list of lists 
extractNth(L,N,L2) :- 
        findall(E,
                (
                member(X,L),
                nth0(N,X,E)
                ),
        L2).
        
%% dot([1,2],[2,3],8).
%% compute dot product between two vectors

dot(A,B,C) :- c(A,B,0,C).

c([],[],C,C).
c([A1|A],[B1|B],C1,R) :- 
        C is C1 + A1*B1,
        c(A,B,C,R).     
        

%% write the List to the output with and each line an  nl

writelnList([]).
writelnList([X|Y]):-
   write(X), nl,
   writelnList(Y).


%% write the List to the output with and additonal nl

writeL(L) :- 
        applyList(write,L),
        nl.
        
writeLTable(L) :-  applyList(write,L).

%% write the List to the output with tab and additonal nl       
        
writeTabL([]) :- nl.
writeTabL([H|T]) :- 
        write(H),
        write('\t'),
        writeTabL(T).

%% write the List to the output with tab and additonal nl       
        
writeTabLRound([]) :- nl.
writeTabLRound([H|T]) :- 
        H2 is round(H),
        write(H2),
        writef('\t'),
        writeTabLRound(T).
        

%% write the List to the output with tab and additonal nl

writeTabLNum([]) :- nl.
writeTabLNum([H|T]) :-
        H2 is round(H),
        write(H2),
        writef('\t'),
        writeTabLNum(T).
        
        
writeMaxList(List,Max):-
   writeMaxList1(List,0,Max),
   nl.
writeMaxList1([],_,_):- !.
writeMaxList1(List,Max,Max):-
   !,nl,
   writeMaxList1(List,0,Max).

writeMaxList1([X|Y],N,Max):-
   write(X),write(' '),
   N1 is N + 1,
   writeMaxList1(Y,N1,Max).

%removes first element
remove(_,[],[]):- !.
remove(X,[X|L1],L1):- !.
remove(X,[Y|L1],[Y|L2]):-
   remove(X,L1,L2).


%% choose(List, Elt) - chooses a random element
%% in List and unifies it with Elt.
choose([], []).
choose(List, Elt) :-
        length(List, Length),
        random(0, Length, Index),
        nth0(Index, List, Elt).

%% shuffle(ListIn, ListOut) - randomly shuffles
%% ListIn and unifies it with ListOut
shuffle([], []):-!.
shuffle(List, [Element|Rest]) :-
        choose(List, Element),
        delete(List, Element, NewList),
        shuffle(NewList, Rest).


