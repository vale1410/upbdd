:-module(orderedSets,[bigUnion/2,ordIntersection/3,numList/3,ordUnion/3,list2ord/2,ordAdd/3,ordDel/3,ordMemberchk/2,ordSubtract/3]).
:-use_module(library(ordsets)).

bigUnion(ListSets,Union):-
   bigUnion(ListSets,[],Union).

bigUnion([],Union,Union):- !.
bigUnion([First|Rest],Temp,Union):-
   ordUnion(First,Temp,Temp2),
   bigUnion(Rest,Temp2,Union).

numList(A,B,L) :- numlist(A,B,L).

list2ord(L,S):- list_to_ord_set(L,S).

ordSubtract(Set,Delete,Remaining) :- ord_subtract(Set,Delete,Remaining).

ordAdd(S,E,SN) :- ord_add_element(S,E,SN).

ordDel(S,E,SN) :- ord_del_element(S,E,SN).

ordMemberchk(E,S) :- ord_memberchk(E,S).

ordUnion(A,B,AB) :- ord_union(A,B,AB).

ordIntersection(A,B,AB) :- ord_intersection(A,B,AB).
