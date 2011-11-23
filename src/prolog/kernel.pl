:-module(kernel,[start/0, test/1]).
:-use_module(rb_transformation_BDD,[buildBDD/3]).
:-use_module(rb_transformation_UPBDD,[buildUPBDD/3]).
:-use_module(bdd,[and/3,satisfiable/2,size/2]).
:-use_module(inout,[getConstraints/2]).
:-use_module(backbone,[initBDD/0, getNode/4,garbageCollection/1,printTable/0]).
:-use_module(utils).
%:-use_module(upbdd,[printTables/0,initUPBDD/0,conjunction/3]).
:-use_module(upbdd_JEAN,[printTables/0,initUPBDD/0,conjunction/3]).
%:-use_module(filenames,[fileSorted/1]).

%files
%format for negative: 5*x_5 == [4,5,0] , -5*x_5 == -5 + 5 (\bar x_5) == [4,5,1]


%file('tests/t1.pl').
%file('tests/t2.pl').
%file('tests/t3.pl').
%file('tests/t4.pl').
%file('tests/t5.pl').
%file('tests/t6.pl').
%file('tests/t7.pl').

file('pigeon/pigeon-2-1.pl').
file('pigeon/pigeon-3-2.pl').
file('pigeon/pigeon-4-3.pl').
file('pigeon/pigeon-5-4.pl').
file('pigeon/pigeon-6-5.pl').
%file('pigeon/pigeon-7-6.pl').
%file('pigeon/pigeon-8-7.pl').

%file('files/prob3.pl').
%file('test2.pl').
%file('files/pigeon-clauses-45-40.pl').
%file('files/pigeon-cardinality-11-10.pl').
%file('files/pigeon-cardinality-15-10.pl').
%file('files/fpga13_13_sat_pb.cnf.cr.pl').
%file('files/elf.rf6.ucl.pl').
%file('files/t2001.13queen13.1110969860.pl').
%file('files/pigeon-cardinality-21-20.pl').
%file('files/robin8.pl').
%file('files/robin10.pl').
%file('files/robin12.pl').
%file('files/robin14.pl').
%file('files/robin16.pl').
%file('files/robin18.pl').

%file('files/cuww1.pl').


test(File):-
   initBDD,
   initUPBDD,
   file(File),
   getConstraints(Constraints,File),nl,
%   shuffle(ConstraintsT,Constraints),
   %Constraints = [
%constraint([[5, 1, 0], [6, 1, 0]], 1),
%constraint([[3, 1, 0], [4, 1, 0]], 1),
%constraint([[2, 1, 1], [4, 1, 1], [6, 1, 1]], 2),
%constraint([[1, 1, 0], [2, 1, 0]], 1),
%constraint([[1, 1, 1], [3, 1, 1], [5, 1, 1]], 2)
%],
%   write(Constraints),nl,
   pseudoToBDDs(Constraints,BDDs),
   pseudoToUPBDDs(Constraints,UPBDDs),
   makeSequentialBDD(BDDs,B),
   makeSequentialUPBDD(UPBDDs,U),
   writeL(['bdd:\t',B,'\tupbdd:\t',U]),
   backbone:id(ID1),
   upbdd_JEAN:id(ID2),
   %upbdd:id(ID2),
   writeL(['bdds:\t',ID1,'\tupbdds:\t',ID2])
   %backbone:printTable,
   %upbdd_JEAN:printTables
   %upbdd:printTables
   .

start:-
  findall(F,file(F),ListOfFiles),
  forall(member(X,ListOfFiles),test(X)).

%solves the problem completely
start1(File):-
   init,
   getConstraints(Constraints,File),
%   shuffle(ConstraintsT,Constraints),
   pseudoToBDDs(Constraints,BDDs1),
 /*
      calculateFirstRound(BDDs1,500,BDDs), nl,
      cleanTableUPBDD,
      forall(member(X,BDDs),(
      size(X,S1),
      transformBDD2UPBDD(X,Y),
      sizeUP(Y,S2),
      sizeUPSET(Y,S3,S4),
   writeL([X,'\t',S1,'\t',S2,'\t',S3-S4])
   )).
 */
   doUPBDDstuff(BDDs1,1000).

 /*  %makeSequentialBDD(BDDs,1,Final),
   makeClusterBDD(BDDs,Final),
   (satisfiable(Final,One)->
   (write(One),nl,
   checkResult(Constraints,One)
   );
   write('non satisfiable')),
   write('##################################################################'),nl,nl.*/

doUPBDDstuff(BDDs1,T):-
   calculateFirstRound(BDDs1,T,BDDs),
   length(BDDs,N),write('\t'),write(N),
   cleanTableUPBDD,
   findall(S1,
      (
      member(X,BDDs),
      size(X,S1)
      ),
      S1T
      ),
   findall(S2-S3,
      (
      member(X,BDDs),
      transformBDD2UPBDD(X,Y),
      sizeUPSET(Y,S2,S3)
      ),
      SXT
      ),
   sumlist(S1T,S1),
   trans(SXT,S2T-S3T),
   sumlist(S2T,S2),
   sumlist(S3T,S3),
   S4 is S3 + S2,
   writeL(['\t',S1,'\t',S4,'\t',S2,'\t',S3]).
   
   
%trans([],[]-[]):-!.
%trans([A-B|R],[A|R1]-[B|R2]):-
%   trans(R,R1-R2).

startSearch:-
   init,
   file(File),
   write(File),nl,
   getConstraints(Constraints,File),
   pseudoToBDDs(Constraints,BDDs1),
   oneRound(BDDs1,BDDs2),
   write(BDDs2),nl,
   %findall(Final,search(BDDs,Final),FinalList),
   search(BDDs2,Final),
   writelnList(Final),nl.


pseudoToBDDs([],[]).
pseudoToBDDs([constraint(Coefficients,Threshold)|CR],[B|BR]):-
   buildBDD(Coefficients,Threshold,B),
   pseudoToBDDs(CR,BR).

pseudoToUPBDDs([],[]).
pseudoToUPBDDs([constraint(Coefficients,Threshold)|CR],[B|BR]):-
   buildUPBDD(Coefficients,Threshold,B),
   pseudoToUPBDDs(CR,BR).

makeSequentialBDD(L,A):-
   makeSequentialBDD(L,1,A).
makeSequentialBDD([],Final,Final) :- !.
makeSequentialBDD([B|BR],Temp,Final):-
   and(NewTemp,Temp,B),
   makeSequentialBDD(BR,NewTemp,Final).
   

makeSequentialUPBDD(L,A):-
   makeSequentialUPBDD(L,1-1,A).
makeSequentialUPBDD([],Final,Final) :- !.
makeSequentialUPBDD([B|BR],Temp,Final):-
   conjunction(Temp,B,NewTemp),
   makeSequentialUPBDD(BR,NewTemp,Final).


makeClusterBDD([B],B) :- !.
makeClusterBDD(BDDs,Final):-
   oneRound(BDDs,Temp),
%   nextRoundwithThreshold(BDDs,1000,Temp),
%   garbageCollection(Temp),
   findall(X,%Size,
   (member(X,Temp)
   %size(X,Size)
   ),
   SizeOfTemp),
   writeMaxList(['next round: '|SizeOfTemp],10),
   makeClusterBDD(Temp,Final).

oneRound([],[]):- !.
oneRound([B],[B]):- !.
oneRound([B1|[B2|BR]],[R|AndR]):-
   backbone:cleanAndBackup, %we dont clean after each call ...
   and(R,B1,B2),
   oneRound(BR,AndR).



%calculates the first round with a threshold

startOutput:-
   writeL(['\\hline']),
   writeL(['name & var & constraints &  100 & 500 &  1000 \\\\']),
%   writeL(['name & var & constraints &  500 & units \\\\']),
   writeL(['\\hline']),
   findall(F,file(F),ListOfFiles),
   forall(
      member(X,ListOfFiles),
       time(startOutputTableWithThresholds(X))
%      startOutputTableWithThresholdsAndUnitPropagation(X)
   ).

startOutputTableWithThresholds(File):-
   init,
   getConstraints(Constraints,File),
   pseudoToBDDs(Constraints,BDDs),
   calculateFirstRound(BDDs,100,Final100),
   calculateFirstRound(BDDs,500,Final500),
   calculateFirstRound(BDDs,1000,Final1000),
   length(Final100,Final100N),length(Final500,Final500N),length(Final1000,Final1000N),
   writeLTable(['\t',Final100N,'\t',Final500N,'\t',Final1000N]),nl.

calculateFirstRound(BDDs,ThresholdSize,Temp):-
   nextRoundwithThreshold(BDDs,ThresholdSize,Temp).

nextRoundwithThreshold(BDDs,ThresholdSize,Finals):-
   nextRoundwithThreshold(BDDs,ThresholdSize,[],Finals).

nextRoundwithThreshold([],_ThresholdSize,Finals,Finals):-!.
nextRoundwithThreshold(BDDs,ThresholdSize,Temp,Finals):-
   backbone:cleanAndBackup,
   BDDs = [First|Rest],
   nextBDDwithThreshold(Rest,RestBDDs,ThresholdSize,First,NewBDD),
%  writeL([NewBDD]),
   nextRoundwithThreshold(RestBDDs,ThresholdSize,[NewBDD|Temp],Finals).
   
nextBDDwithThreshold([],[],_ThresholdSize,BDDFinal,BDDFinal):- !.
%nextBDDwithThreshold([BDDFinal],[],_ThresholdSize,BDDFinal,BDDFinal):- !.
nextBDDwithThreshold([NextBDD|BDDs],RestBDDs,ThresholdSize,BDDNow,BDDFinal):-
   size(BDDNow,Size1),
   size(NextBDD,Size2),
   Size is 2*(Size1 + Size2),
   (ThresholdSize < Size ->
   (BDDNow = BDDFinal,
   [NextBDD|BDDs] = RestBDDs
   );
   (
   and(NewBDD,NextBDD,BDDNow),
   nextBDDwithThreshold(BDDs,RestBDDs,ThresholdSize,NewBDD,BDDFinal)
   )).


%:-use_module(<transformation,[buildBDD/3]).
%:-use_module(avl_transformation,[buildBDD/3]).
%:-use_module(search,[search/2]).
%:-use_module(minimizer).
%:-use_module(upbddcount).
%:-use_module(checkResults,[checkResult/2]).
%:-use_module(unitpropagation,[unitPropagation/4,unitPropagation/3]).
