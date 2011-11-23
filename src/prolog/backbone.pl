:-module(backbone,[
                   initBDD/0,exists/5,getEntry/6,insert/6,generateId/4,addNode/4,
                   getNode/4,printTable/0,cleanTable/0,addAnd/3,getAnd/3,cleanAndBackup/0,
                   printAndBackup/0,cleanTransformTable/0,garbageCollection/1, getLevels/1,setLevels/1,
                   getMinimizer/2
                   ]).
:-use_module(utils).
:-use_module(orderedSets).
%node(ID,Level,High,Low,Extra) Extra = [Models]
%nodebackup keeps information via (High,Low) if node already exists!
:-dynamic node/5,id/1,andBackup/2,nodeBackup/2, entry/6. %entry(id,level,lb,ub,one,zero).


getLevels(X):-
   flag(levels,X,X).
setLevels(X):-
   flag(levels,_,X),
   retract(node(0,_10000000,x,x,[_,_])),
   retract(node(1,_10000000,x,x,[_,_])),
   assert(node(0,X,x,x,[0,[]])),
   assert(node(1,X,x,x,[1,[]])).

initBDD :-
   cleanTable,
   %cleanTransformTable,
   cleanAndBackup.

generateId(Id):-
   generateId(Id,_Var,_High,_Low).

generateId(Id,_Var,_High,_Low):-
%   Tmp is ((High + Low)*(High + Low+1)*(1/2))+High,
%   Id is mod((((Var + Tmp)*(Var + Tmp+1))/2)+Var,15485863).
   (
   id(X)->
   (
   retract(id(X)),
   Id is X + 1,
   %testID(Id),
   asserta(id(Id))
   );
   Id = 2,
   asserta(id(2))
   ).
   
testID(ID):-
   (0 is ID mod 100000  ->
   (write('id: '),write(ID),nl);
   true).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55
%  garbage collection with ordred lists
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55

garbageCollection(BDDs):-
   garbageCollection2(BDDs).
%   garbageCollection1(BDDs).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55
%  garbage collection without lists
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55

garbageCollection2(BDDsIN):-
   list2ord(BDDsIN,BDDs),
   writeL(['gc: start']),
   findall(X,node(X,_,_,_,_),AllNodes1),
   reverse(AllNodes1,AllNodes),
   length(AllNodes,L2),writeL(['gc: table has ', L2 ,' nodes']),
   collectExistingNodes2(BDDs,AllNodes,Rest),
   writeL(['gc: start retraction']),
   forall(member(X,Rest),retract(node(X,_,_,_,_))),
   length(Rest,L3),writeL(['gc: deleting ', L3 ,' nodes from table']),
   writeL(['gc: finished'])
   .
   
collectExistingNodes2([],Final,Final):- !.
collectExistingNodes2([First|BDDs],Temp,Final):-
   (removeWithFail(First,Temp,Temp2)->
      (
      getNode(First,_Var,High,Low),
      %ordAdd(BDDs,High,BDDs1),
      %ordAdd(BDDs1,Low,BDDs2),
      collectExistingNodes2([High,Low|BDDs],Temp2,Final)
      );
      collectExistingNodes2(BDDs,Temp,Final)
   ).

%removes first element
%removeWithFail(_,[],[]):- !.
removeWithFail(X,[X|L1],L1):- !.
removeWithFail(X,[Y|L1],[Y|L2]):-
   removeWithFail(X,L1,L2).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55
%  garbage collection with ordred lists
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55

garbageCollection1(BDDs):-
   writeL(['gc: start']),
   list2ord(BDDs,OrdList),
   writeL(['gc: start collectExistingNodes']),
   collectExistingNodes(OrdList,[0,1],All1),
   writeL(['gc: start table to set']),
   length(All1,L1),writeL(['gc: the programm is referencing ', L1 ,' nodes']),
   findall(X,node(X,_,_,_,_),All2),
   length(All2,L2),writeL(['gc: table has ', L2 ,' nodes']),
   list2ord(All2,All21),
   writeL(['gc: start substraction']),
   ordSubtract(All21,All1,All3),
   writeL(['gc: start retraction']),
   forall(member(X,All3),retract(node(X,_,_,_,_))),
   length(All3,L3),writeL(['gc: deleting ', L3 ,' nodes from table']),
   writeL(['gc: finished'])
   .

collectExistingNodes([],Final,Final):- !.
collectExistingNodes([First|BDDs],Temp,Final):-
   (ordMemberchk(First,Temp)->
      collectExistingNodes(BDDs,Temp,Final);
      (
      getNode(First,_Var,High,Low),
      ordAdd(Temp,First,Temp2),
      collectExistingNodes([High,Low|BDDs],Temp2,Final)
      )
   ).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55
%  BDD Hashmap
% two tables: node, nodeBackup (if node exists in table)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55

node(1,10000000,x,x,[1,[]]).
node(0,10000000,x,x,[0,[]]).

cleanTable :-
  retractall(node(_,_,_,_,_)),
  retractall(nodeBackup(_,_)),
  (id(_) -> retract(id(_));true),
  asserta(node(1,10000000,x,x,[1,_])),
  asserta(node(0,10000000,x,x,[0,_])).

addNode(Same,_,Same,Same):-
   !.

%check if it exists!
%make a table for it:


addNode(Id,Var,High,Low):-
   pairing(High,Low,Hash),
   nodeBackup(Hash,Id),
   node(Id,Var,High,Low,_),
%   write('.'),
   !.
   
addNode(Id,Var,High,Low):-
   generateId(Id,Var,High,Low),
   pairing(High,Low,Hash),
   asserta(nodeBackup(Hash,Id)),
   computeExtraInformation(Var,High,Low,Extra),
   asserta(node(Id,Var,High,Low,Extra)).


getExtraInformation(Id,Var,Extra):-
   node(Id,Var,_,_,Extra).
   
getMinimizer(Id,Minimizer):-
   node(Id,_,_,_,[_,Minimizer]),
   !.

% until now only size and models
computeExtraInformation(CurrentLevel,High,Low,[_Models,Propagation]):-
   getExtraInformation(High,_VarHigh,[_ModelHigh,PropagatorH]),
   getExtraInformation(Low,_VarLow,[_ModelLow,PropagatorL]),
   (nth0(Direction,[High,Low],0)->
      (
      nth0(Direction,[PropagatorL,PropagatorH],Child),
      union([CurrentLevel-Direction],Child,Propagation)
      );
   intersection(PropagatorH,PropagatorL,Propagation)
   ),
   !.
%   Models is 2^(VarHigh - CurrentLevel -1)*ModelHigh + 2^(VarLow - CurrentLevel -1)*ModelLow.
   


   
   
   
pairing(X,Y,Z):-
   Z is (((X + Y)*(X + Y + 1))/2)+ X.
   
getNode(Id,Var,High,Low):-
   node(Id,Var,High,Low,_),
   !.
   
printTable :-
   findall([A,B,C,D],node(A,B,C,D,_E),X),
   applyList(writeTabL,X).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55
%  AND TABELING
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55


cleanAndBackup:-
   retractall(andBackup(_,_)).
   
getAnd(X,Y,Z):-
   XY is ((max(X,Y)-1)*(max(X,Y)-2))/2 + min(X,Y) - 1,
   andBackup(XY,Z).
   
addAnd(X,Y,Z):-
   XY is ((max(X,Y)-1)*(max(X,Y)-2))/2 + min(X,Y) - 1,
   asserta(andBackup(XY,Z)).
   
printAndBackup:-
   findall([A,B],andBackup(A,B),X),
   applyList(writeTabL,X).
   

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55
%  TRANSFORMATION TABLENING
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%55

   
getEntry(ID,Level,LB,UB,ID1,ID2):-
   entry(ID,Level,LB,UB,ID1,ID2).

exists(Value,Level,ID,LB,UB):-
   entry(ID,Level,LB,UB,_One,_Zero),
   LB =< Value,
   Value =< UB,
   !.

%workaround!

insert(Level,LB,UB,0,0,0):-
   asserta(entry(0,Level,LB,UB,0,0)),
   !.

%critical thing!
insert(_Level,_LB,_UB,X,X,X):-
%   entry(Level,LB,UB,X,One,Zero),
%   asserta(entry(x,Level,LB,UB,One,Zero)),
   !.

insert(Level,LB,UB,One,Zero,ID):-
   addNode(ID,Level,One,Zero),
   asserta(entry(ID,Level,LB,UB,One,Zero)).

cleanTransformTable :-
  retractall(entry(_,_,_,_,_,_)).
  
  
