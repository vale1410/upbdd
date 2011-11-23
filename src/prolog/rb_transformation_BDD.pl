:-module(rb_transformation_BDD,[buildBDD/3]).
:-use_module(backbone,[addNode/4]).
:-use_module(redblackTrees,[rb_new/1,rb_my_lookup/4,rb_insert/4]).
:-use_module(utils).

buildBDD(NormalisedCoefficients,Threshold,TOPNode):-
   T is -Threshold,
   buildBDD(NormalisedCoefficients,T,TOPNode,_LB,_UB,[],_ListOfTreesOUT).

buildBDD(_Coefficients,Value,1,0,1000000000000000,ListOfTreesIN,ListOfTreesIN):-
   Value >= 0,
   !.
buildBDD([],_Value,0,-1000000000000000,-1,ListOfTreesIN,ListOfTreesIN):- !.

buildBDD([[Level,_,_]|_],Value,ID,LB,UB,ListOfTreesIN,ListOfTreesIN):-
   exists(Value,Level,ID,LB,UB,ListOfTreesIN),
   !.

buildBDD([[NewLevel,Coefficient,Parity]|RestCoefficients],Value,ID,LB,UB,ListOfTreesIN,ListOfTreesOUT):-
   NewValue is Value + Coefficient,
   buildBDD(RestCoefficients,NewValue,ID1,LB1,UB1,ListOfTreesIN,ListOfTreesIN2),
   buildBDD(RestCoefficients,Value,ID2,LB2,UB2,ListOfTreesIN2,ListOfTreesIN3),
   LB1X is LB1 - Coefficient,
   UB1X is UB1 - Coefficient,
   LB is max(LB1X,LB2),
   UB is min(UB1X,UB2),
   (Parity = 0 ->
   insert(NewLevel,LB,UB,ID1,ID2,ID,ListOfTreesIN3,ListOfTreesOUT);
   insert(NewLevel,LB,UB,ID2,ID1,ID,ListOfTreesIN3,ListOfTreesOUT)
   ).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  TRANSFORMATION AVLTREE
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

insert(Level,LB,UB,0,0,0,ListOfTreesIN,ListOfTreesOUT):-
   insertion([Level,LB,UB],[0,0,0],ListOfTreesIN,ListOfTreesOUT),
%   asserta(entry(0,Level,LB,UB,0,0)),
   !.

%critical thing!
insert(_Level,_LB,_UB,X,X,X,ListOfTreesIN,ListOfTreesIN):-
   !.

insert(Level,LB,UB,One,Zero,ID,ListOfTreesIN,ListOfTreesOUT):-
   addNode(ID,Level,One,Zero),
   insertion([Level,LB,UB],[ID,One,Zero],ListOfTreesIN,ListOfTreesOUT).
%   asserta(entry(ID,Level,LB,UB,One,Zero)).



replace([Level,TCREATE],[],TNEW,[[Level,TNEW]]):-
   rb_new(TCREATE),
   !.
replace([Level,TIN],[[Level,TIN]|Rest],TNEW,[[Level,TNEW]|Rest]):-!.
replace(Z,[X|Rest1],TNEW,[X|Rest2]):-
   replace(Z,Rest1,TNEW,Rest2).


%in,in,in,out,
insertion([Level,LB,UB],[ID,One,Zero],ListOfTreesIN,ListOfTreesOUT):-
   replace([Level,TIN],ListOfTreesIN,TOUT,ListOfTreesOUT),
   insertion(LB,UB,[ID,One,Zero],TIN,TOUT).

%in,in,in,in,in,out,out,
insertion(LB,UB,Data,TIN,TOUT):-
    rb_insert(TIN,[LB,UB],Data,TOUT).


exists(Value,Level,ID,LB,UB,ListOfTreesIN):-
   (memberchk([Level,Tree],ListOfTreesIN)->
   lookup(Value,Tree,[ID,_,_],LB,UB)
   ;
   fail).


lookup(Value,Tree,Data,LB,UB):-
   rb_my_lookup(Value,Data,Tree,[LB,UB]).
