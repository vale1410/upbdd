:-module(bdd,[and/3,satisfiable/2,size/2,variables/2]).
:-use_module(backbone,[addNode/4,getNode/4,getAnd/3,addAnd/3]).
:-use_module(redblackTrees).
:-use_module(orderedSets).
:-use_module(utils).


%export Predicate:
%   and(BDDOUT,BDDIN,BDDIN).
%   satisfiable(BDD,Valuation).
%   size(BDD,N)
%   variables(BDD,List(int))

% gives out all variables occuring in the bdd


variables(BDD,Variable):-
   rb_new(CheckedBDDsNew),
   backbone:getLevels(X),
   variables(BDD,[],Variable,CheckedBDDsNew,_CheckedBDDs,X).
%   utils:writeL(['vc: ',N]).

variables(0,F,F,B,B,_) :- !.
variables(1,F,F,B,B,_) :- !.
variables(BDD,VIN,VOUT,BDDIN,BDDOUT,TotalNLevels) :-
   (rb_lookup(BDD,_,BDDIN)->
      (
      VIN=VOUT,
      BDDIN=BDDOUT
      )
   ;
      (
      backbone:getNode(BDD,Level,ONE,ZERO),
      rb_insert(BDDIN,BDD,_,BDDIN1),
      ordAdd(VIN,Level,VIN1),
      length(VIN1,Length),
        (Length is TotalNLevels - Level ->
         (
         VIN1=VOUT,
         BDDIN1=BDDOUT
         );
         (
         variables(ONE,VIN1,VIN2,BDDIN1,BDDIN2,TotalNLevels),
         variables(ZERO,VIN2,VOUT,BDDIN2,BDDOUT,TotalNLevels)
         )
        )
      )
   ).

/*
variables(BDD,Variable):-
   variables(BDD,[],Variable,[],_CheckedBDDs,0,N),
   writeL(['vc: ',N]).

variables(0,F,F,B,B,N,N) :- !.
variables(1,F,F,B,B,N,N) :- !.
variables(BDD,VIN,VOUT,BDDIN,BDDOUT,M,N) :-
    M1 is M + 1,
    backbone:testID(M1),
   (memberchk(BDD,BDDIN)->
      (
      VIN=VOUT,
      BDDIN=BDDOUT,
      M1 = N
      )
   ;
      (
      getNode(BDD,Level,ONE,ZERO),
      ordAdd(VIN,Level,VIN2),
      variables(ONE,VIN2,VIN3,[BDD|BDDIN],BDDIN2,M1,M2),
      variables(ZERO,VIN3,VOUT,BDDIN2,BDDOUT,M2,N)
      )
   ).
*/


%better if i would calculate instantly the number of nodes and not though length
%predicate.

size(BDD,N):-
   size2(BDD,N).
%   time(size2(BDD,N)),
%   time(size3(BDD,N)).


size1(BDD,N):-
   size1(BDD,[],NOUT),
   length(NOUT,N).

size1(0,NIN,NIN):- !.
size1(1,NIN,NIN):- !.
size1(BDD,NIN,NOUT):-
   (memberchk(BDD,NIN)->
   (NIN=NOUT);
   getNode(BDD,_,B1,B2),
   %ordAdd(NIN,BDD,NIN2),
   NIN2 = [BDD|NIN],
   size1(B1,NIN2,NIN3),
   size1(B2,NIN3,NOUT)
   ).




size2(BDD,SIZE):-
   rb_new(BDDIN),
   size2(BDD,BDDIN,_BDDOUT,0,SIZE).

size2(0,B,B,N,N) :- !.
size2(1,B,B,N,N) :- !.
size2(BDD,BDDIN,BDDOUT,M,N) :-
   (rb_lookup(BDD,_,BDDIN)->
      (
      BDDIN=BDDOUT,
      M = N
%      ,writeL([BDD,'\t',M])
      )
   ;
      (M1 is M + 1,
      backbone:getNode(BDD,_,ONE,ZERO),
      rb_insert(BDDIN,BDD,0,BDDIN1),
      size2(ONE,BDDIN1,BDDIN2,M1,M2),
      size2(ZERO,BDDIN2,BDDOUT,M2,N)
      )
   ).



size3(BDD,SIZE):-
   size3(BDD,[],_BDDOUT,0,SIZE).

size3(0,B,B,N,N) :- !.
size3(1,B,B,N,N) :- !.
size3(BDD,BDDIN,BDDOUT,M,N) :-
   (ordMemberchk(BDD,BDDIN)->
      (
      BDDIN=BDDOUT,
      M = N
      )
   ;
      (M1 is M + 1,
      getNode(BDD,_,ONE,ZERO),
      ordAdd(BDDIN,BDD,BDDIN1),
      size3(ONE,BDDIN1,BDDIN2,M1,M2),
      size3(ZERO,BDDIN2,BDDOUT,M2,N)
      )
   ).


satisfiable(BDD,Valuation):-
   satisfiable(BDD,[],Valuation).

satisfiable(1,F,F2) :- !,reverse(F,F2).
satisfiable(0,_F,_F2) :- !,fail.
satisfiable(B,ACCU,Final):-
   getNode(B,Var,B1,B2),
   (satisfiable(B1,[[Var,1]|ACCU],Final);
   satisfiable(B2,[[Var,0]|ACCU],Final)).

and(B,B,B) :- !.
and(0,0,_) :- !.
and(0,_,0) :- !.
and(B,1,B) :- !.
and(B,B,1) :- !.
%tabeling
and(B,B1,B2) :-
   getAnd(B1,B2,B),
   %flag(x,N,0),
   %M is N + 1,
   %write(N),nl,
   %flag(x,_,M),
   !.
   
and(R,B1,B2):-
   %write([B1,B2]),nl,
   getNode(B1,L1,High1,Low1),
   getNode(B2,L2,High2,Low2),
   (L1==L2 ->
       and(High3,High1,High2),
       and(Low3,Low1,Low2),
       addNode(R,L1,High3,Low3)
   ;
       (L1<L2 ->
          and(High3,High1,B2),
          and(Low3,Low1,B2),
          addNode(R,L1,High3,Low3)
       ;
          and(High3,B1,High2),
          and(Low3,B1,Low2),
          addNode(R,L2,High3,Low3)
       )
   ),
   %saving for tabeling
   addAnd(B1,B2,R).
