:-module(upbdd,[assertUPBDD/4,initUPBDD/0,printTables,conjunction/3]).
:-dynamic upNode/4,setNode/3,id/1.

/*
********************************************************************************
table operations, contains reductions rules as well
********************************************************************************
*/

% set(ID,VAR-D,NEXT).
% upNode(ID,VAR,HIGH,LOW)

%initializes tables
initUPBDD :-
  retractall(upNode(_,_,_,_)),
  retractall(setNode(_,_,_)),
  retractall(id(_)),
  asserta(setNode(1,x-x,x)).

%generates a unique id
genId(Id):-
   (id(X)->
      (
      retract(id(X)),
      Id is X + 1,
      asserta(id(Id))
      );
      Id = 2,
      asserta(id(2))
   ).

%simple node handling of setNodes
assertSet(0,_,0):- !.
assertSet(ID,First,RestID):-
   (setNode(ID2,First,RestID)->
   ID = ID2;
      (
      genId(ID),
      asserta(setNode(ID,First,RestID))
      )
   ).



assertUPBDD(UP,_,UP,UP):- !. %reduction rule 1
assertUPBDD(ZERO-UPI,VAR,0-_,ZERO-SETZERO):-
   !,
   assertSet(UPI,VAR-0,SETZERO).
assertUPBDD(ONE-UPI,VAR,ONE-SETONE,0-_):-
   !,
   assertSet(UPI,VAR-1,SETONE).
assertUPBDD(UP-UPI,VAR,ONE-SETONE,ZERO-SETZERO):-
   (upNode(ID,Var,ONE-SETONE,ZERO-SETZERO)->
      (
      ID = UP,
      UPI = 1
      )
      ;
      (
      intersectSets(SETONE,SETZERO,UPI),
      subtractSets(SETONE,UPI,SETONE2),
      subtractSets(SETZERO,UPI,SETZERO2),
         (
         upNode(ID,Var,ONE-SETONE2,ZERO-SETZERO2)->
         ID = UP;
            (
            genId(UP),
            asserta(upNode(UP,VAR,ONE-SETONE2,ZERO-SETZERO2))
            )
         )
      )
   ).
/*
********************************************************************************
lazy disjunction
********************************************************************************
*/
disjunction(F,G,R) :-
        disjunction(F,G,1,1,R).

%base cases
disjunction(1-1,_,1,_,1-1):- !.
disjunction(_,1-1,_,1,1-1):- !.
disjunction(0-_,G,_,GFIX,R):-
   !,
   instantiate(G,GFIX,R).
disjunction(F,0-_,FFIX,_,R):-
   !,
   instantiate(F,FFIX,R).

disjunction(F-1,G-1,FFIX,GFIX,R-RUP) :-
        % this case makes sure that FFIX and GFIX dont intersect!
   intersectSets(FFIX,GFIX,INT),
   INT \= 1, !,
   subtractSets(FFIX,INT,FREST),
   subtractSets(GFIX,INT,GREST),
   disjunction(F-1,G-1,FREST,GREST,R-RUP1),
   conjunctSets(RUP1,INT,RUP).
disjunction(F-1,G-1,FFIX,GFIX,R-RUP):-
   !,
   ( upNode(F,VF1,HIGHF,LOWF) -> true ; VF1 = y ), %% hack to cope with F = 1
   setNode(FFIX,VF2-DF,NEXTFFIX),
   ( upNode(G,VG1,HIGHG,LOWG) -> true ; VG1 = y ), %% hack to cope with G = 1
   setNode(GFIX,VG2-DG,NEXTGFIX),
   (VF2 @< VF1 ->
       (VG2 @< VG1 ->
           (VF2 = VG2 ->
               %% cannot have the same sign otherwise removed by previous clause
               %% split on VF2!
               %  question do I need to  instantiate(F-NEXTFFIX) etc.?????
               nth0(DF,[G-NEXTGFIX,F-NEXTFFIX], HIGH),
               nth0(DG,[G-NEXTGFIX,F-NEXTFFIX], LOW),
               assertUPBDD(R-RUP, VF2, HIGH, LOW)
           ; VF2 @< VG2 ->
               disjunction(F-1,G-1,NEXTFFIX,GFIX,R1),
               nth0(DF,[G-GFIX,R1],HIGH),
               nth0(DF,[R1,G-GFIX],LOW),
               assertUPBDD(R-RUP,VF2,HIGH,LOW)
           ; % VG2 @< VF2 ->
               disjunction(F-1,G-1,FFIX,NEXTGFIX,R1),
               nth0(DG,[F-FFIX,R1],HIGH),
               nth0(DG,[R1,F-FFIX],LOW),
               assertUPBDD(R-RUP,VG2,HIGH,LOW)
           )
       ; VG2 = VG2 ->
           % cannot happen
           abort
       ; % VG1 @< VG2 ->
           ( VF2 = VG1 ->
           % fixed in F, not in G
               (DF = 1 ->
                   disjunction(F-1, HIGHG, NEXTFFIX, GFIX, HIGH),
                   assertUPBDD(R-RUP,VF2,HIGH,LOWG-GFIX)
               ;
                   disjunction(F-1, LOWG, NEXTFFIX, GFIX, LOW),
                   assertUPBDD(R-RUP, VF2, HIGHG-GFIX, LOW)
               )
           ; VF2 @< VG1 ->
               % split on VF2
               disjunction(F-1,G-1,NEXTFFIX,GFIX,R1),
               nth0(DF,[G-GFIX,R1],HIGH),
               nth0(DF,[R1,G-GFIX],LOW),
               assertUPBDD(R-RUP,VF2,HIGH,LOW)
           ; % VG1 @< VF2 ->
               % split on VG1
               disjunction(F-1, LOWG, FFIX, GFIX, LOW),
               disjunction(F-1, HIGHG, FFIX, GFIX, HIGH),
               assertUPBDD(R-RUP, VG1, HIGH, LOW)
           )
       )
   ; VF2 = VF1 ->
       % should not happen
       abort
   ; % VF1 @< VF2 ->
       (VG2 @< VG1 ->
           (VF1 = VG2 ->
               (DG = 1 ->
                   disjunction(HIGHF, G-1, FFIX, NEXTGFIX, HIGH),
                   assertUPBDD(R-RUP,VG2,HIGH,LOWF-FFIX)
               ;
                   disjunction(LOWF, G-1, FFIX, NEXTGFIX, LOW),
                   assertUPBDD(R-RUP, VG2, HIGHF-FFIX, LOW)
               )
           ; VG2 @< VF1 ->
               % split on GFIX
               disjunction(F-1,G-1,FFIX,NEXTGFIX,R1),
               nth0(DG,[F-FFIX,R1],HIGH),
               nth0(DG,[R1,F-FFIX],LOW),
               assertUPBDD(R-RUP,VG2,HIGH,LOW)
           ; % VF1 @< VG2 ->
               % spit on VF1
               disjunction(LOWF, G-1, FFIX, GFIX, LOW),
               disjunction(HIGHF, G-1, FFIX, GFIX, HIGH),
               assertUPBDD(R-RUP, VF1, HIGH, LOW)
           )
       ; % VG1 @< VG2 ->
           % most general case
           (VF1 = VG1 ->
                disjunction(LOWF,LOWG,FFIX,GFIX,LOW),
                disjunction(HIGHF,HIGHG,FFIX,GFIX,HIGH),
                assertUPBDD(R-RUP, VF1, HIGH, LOW)
           ; VF1 @< VG1 ->
               disjunction(LOWF, G-1, FFIX, GFIX, LOW),
               disjunction(HIGHF, G-1, FFIX, GFIX, HIGH),
               assertUPBDD(R-RUP, VF1, HIGH, LOW)
           ; % VG1 @< VF1 ->
               disjunction(F-1, LOWG, FFIX, GFIX, LOW),
               disjunction(F-1, HIGHG, FFIX, GFIX, HIGH),
               assertUPBDD(R-RUP, VG1, HIGH, LOW)
           )
       )
   ).


%% the next two cases simply strip the fixed sets from the first two arguments

disjunction(F-1,G-GUP,FFIX,GFIX,R-RUP) :-
   !,
   conjunctSets(GUP, GFIX, GFIX2),
   disjunction(F-1,G-1,FFIX,GFIX2,R-RUP).

disjunction(F-FUP,G-GUP,FFIX,GFIX,R-RUP) :-
   !,
   conjunctSets(FUP, FFIX, FFIX2),
   disjunction(F-1,G-GUP,FFIX2,GFIX,R-RUP).


/*
********************************************************************************
lazy conjunction
********************************************************************************
*/

conjunction(F,G,R):-
   conjunction(F,G,1,1,R).

%base cases
conjunction(0-_,_,_,_,0-0):- !.
conjunction(_,0-_,_,_,0-0):- !.
conjunction(1-1,G,_,GFIX,R):-
   !,
   instantiate(G,GFIX,R).
conjunction(F,1-1,FFIX,_,R):-
   !,
   instantiate(F,FFIX,R).


% case that there are no UPs, usual conjunction,
% but we need to check the fixed sets first
conjunction(F-1,G-1,FFIX,GFIX,R-RUP):-
   !,
   upNode(F,VF1,HIGHF,LOWF),
   setNode(FFIX,VF2-DF,NEXTFFIX),
   ((FFIX\=1,VF1=VF2) ->
      (
        nth0(DF,[LOWF,HIGHF],NEXTF),
        conjunction(NEXTF,G-1,NEXTFFIX,GFIX,R-RUP)
      );
    (FFIX\=1,VF1>VF2) ->
      (
        conjunction(F-1,G-1,NEXTFFIX,GFIX,R-RUP)
      );
      (
      upNode(G,VG1,HIGHG,LOWG),
      setNode(GFIX,VG2-DG,NEXTGFIX),
       ((GFIX\=1,VG1=VG2) ->
         (
            nth0(DG,[LOWG,HIGHG],NEXTG),
            conjunction(F-1,NEXTG,FFIX,NEXTGFIX,R-RUP)
         );
         (GFIX\=1,VG1>VG2) ->
         (
            conjunction(F-1,G-1,FFIX,NEXTGFIX,R-RUP)
         );
         (
            (
            VF1<VG1 ->
              (
              conjunction(HIGHF,G-1,FFIX,GFIX,HIGHR),
              conjunction(LOWF,G-1,FFIX,GFIX,LOWR),
              assertUPBDD(R-RUP,VF1,HIGHR,LOWR)
              )
            ;
            VF1>VG1 ->
              (
              conjunction(F-1,HIGHG,FFIX,GFIX,HIGHR),
              conjunction(F-1,LOWG,FFIX,GFIX,LOWR),
              assertUPBDD(R-RUP,VF1,HIGHR,LOWR)
              )
            ;
              (
              conjunction(HIGHF,HIGHG,FFIX,GFIX,HIGHR),
              conjunction(LOWF,LOWG,FFIX,GFIX,LOWR),
              assertUPBDD(R-RUP,VF1,HIGHR,LOWR)
              )
            )
         )
       )
      )
  ).

%general case:
% 1) conjuntion of FUP /\ GUP wrt.  FFIX and GFIX
% 2) recursive call to conjunction of nonfix parts wrt. updated FFIX2,GFIX2
conjunction(F-FUP,G-GUP,FFIX,GFIX,R-RUP):-
   specialSetCon(FUP,GUP,FFIX,GFIX,RUP1,FFIX2,GFIX2),
   (RUP1 = 0 ->
    (R = 0,RUP = 0)
    ;
    (
    conjunction(F-1,G-1,FFIX2,GFIX2,R-RUP2),
    (R = 0 ->
     RUP = 0;
     conjunctSets(RUP1,RUP2,RUP)
    )
    )
   ).

%RUP = FUP|_FFIX /\ GUP|_GFIX
%FFIX2 = FFIX /\ GUP,
%GFIX2 = GFIX /\ FUP,
%for now quick and dirty, without smart recursion aso.
specialSetCon(FUP,GUP,FFIX,GFIX,RUP,FFIX2,GFIX2):-
   instantiateSets(FUP,FFIX,RUP1),
   instantiateSets(GUP,GFIX,RUP2),
   conjunctSets(RUP1,RUP2,RUP),
   conjunctSets(FFIX,GUP,FFIX2),
   conjunctSets(GFIX,FUP,GFIX2).


%C = A|_B, A is UPBDD, B is set
instantiate(A,1,A):- !.
instantiate(1-A,B,C-CUP):-
   !,
   instantiateSets(A,B,CUP),
   (CUP = 0 -> C = 0;C = 1).

instantiate(A-1,B,C):-
   !,
   upNode(A,VA,HIGHA,LOWA),
   setNode(B,VB-DB,NEXTB),
   (VA<VB ->
      (
      instantiate(HIGHA,B,HIGHC),
      instantiate(LOWA,B,LOWC),
      assertUPBDD(C,VA,HIGHC,LOWC)
      );
    (VA>VB ->
    instantiate(A-1,NEXTB,C);
      (
      nth0(DB,[LOWA,HIGHA],NEXTA),
      instantiate(NEXTA,NEXTB,C)
      )
    )
   ).

instantiate(A-AUP,B,C-CUP):-
   instantiateSets(AUP,B,CUP1),
   (CUP1 = 0 ->
     (C = 0,CUP = 0);
     (
     instantiate(A-1,B,C-CUP2),
       (C = 0 ->
        CUP = 0;
       conjunctSets(CUP1,CUP2,CUP)
       )
     )
   ).

/*
********************************************************************************
Set operations
********************************************************************************
*/

%C = A|_B
instantiateSets(A,1,A):- !.
instantiateSets(1,_,1):- !.
instantiateSets(A,B,C):-
   setNode(A,VA-DA,NEXTA),
   setNode(B,VB-DB,NEXTB),
   (VA<VB ->
      (
      instantiateSets(NEXTA,B,NEXTC),
      assertSet(C,VA-DA,NEXTC)
      );
    VA>VB -> instantiateSets(A,NEXTB,C);
    (DA=DB ->
     instantiateSets(NEXTA,NEXTB,C)
     ;
     C = 0
    )
   ).


% C = A /\ B, returns 1 if conjunction is empty, returns 0 if contradiciton occurs
conjunctSets(1,B,B):- !.
conjunctSets(A,1,A):- !.
conjunctSets(A,A,A):- !.
conjunctSets(A,B,C):-
   setNode(A,VA-DA,NEXTA),
   setNode(B,VB-DB,NEXTB),
   (VA<VB ->
      (
      conjunctSets(NEXTA,B,NEXTC),
      assertSet(C,VA-DA,NEXTC)
      );
    VA>VB ->
      (
      conjunctSets(A,NEXTB,NEXTC),
      assertSet(C,VB-DB,NEXTC)
      );
    (DA=DB ->
        (
        conjunctSets(NEXTA,NEXTB,NEXTC),
        assertSet(C,VA-DA,NEXTC)
        );
        C = 0
     )
   ).


% C = A \cap B
intersectSets(1,_,1):- !.
intersectSets(_,1,1):- !.
intersectSets(A,A,A):- !.
intersectSets(A,B,C):-
   setNode(A,VA-DA,NEXTA),
   setNode(B,VB-DB,NEXTB),
   (VA<VB -> intersectSets(NEXTA,B,C);
    VA>VB -> intersectSets(A,NEXTB,C);
    (DA=DB ->
        (
        intersectSets(NEXTA,NEXTB,NEXTC),
        assertSet(C,VA-DA,NEXTC)
        );
        intersectSets(NEXTA,NEXTB,C)
     )
   ).

% C = A - B
subtractSets(1,_,1):- !.
subtractSets(A,1,A):- !.
subtractSets(A,A,1):- !.
subtractSets(A,B,C):-
   setNode(A,VA-DA,NEXTA),
   setNode(B,VB-DB,NEXTB),
   (VA<VB ->
      (
      subtractSets(NEXTA,B,NEXTC),
      assertSet(C,VA-DA,NEXTC)
      );
    VA>VB -> subtractSets(A,NEXTB,C);
    (DA=DB ->
        subtractSets(NEXTA,NEXTB,C);
        (
        subtractSets(NEXTA,NEXTB,NEXTC),
        assertSet(C,VA-DA,NEXTC)
        )
     )
   ).

%adds a set [VAR-D|...] and returns ID to top node
addSet([],1).

addSet([X|Y],ID):-
   addSet(Y,ID2),
   assertSet(ID,X,ID2).

/*
********************************************************************************
printing stuff
********************************************************************************
*/

printTables:-
   write('upbdd:'),nl,
   writeL([id,'\t',var,'\t',one-set,'\t',zero-set]),
   printTableUPBDD,
   write('sets:'),nl,
   writeL(['id','\t','var-d','\t','next']),
   printTableSet.

printTableUPBDD:-
   forall(upNode(ID2,Var,ONE-SETONE,ZERO-SETZERO),
   writeL([ID2,'\t',Var,'\t',ONE-SETONE,'\t',ZERO-SETZERO])
   ).

printTableSet:-
   forall(setNode(ID,Node,Rest),
   writeL([ID,'\t',Node,'\t',Rest])).

writeL([]) :- !,nl.
writeL([H|T]) :-
   write(H),
   writeL(T).

/*
********************************************************************************
examples
********************************************************************************
*/
%% LEFT : X2 /\ (X1 xor X3)
%% RIGHT : X1 /\ X2 /\ X3
%% RESULT : 0-0
test1 :-
   initUPBDD,
   %right
   addSet([2-1,3-0],X0),
   addSet([2-1,3-1],X1),
   assertUPBDD(RIGHT,1,1-X0,1-X1),
   %left
   addSet([1-1,2-1,3-1],T),
   LEFT = 1-T,
   conjunction(RIGHT,LEFT,RESULT),
   writeL(['right ',RIGHT,' left ',LEFT,' result: ',RESULT]),
   printTables.

%% LEFT : ((X1 /\ X5) -> X3) /\ (X4 -> X2)
%% RIGHT : X1 /\ X5 /\ (X3 -> X2)
%% RESULT : X1 /\ X2 /\ X3 /\ X5
test2 :-
   initUPBDD,
   %left
   addSet([4-0],X40),
   addSet([5-0],X50),
   assertUPBDD(X3-1,3,1-1,1-X50),
   assertUPBDD(X21,2,X3-1,X3-X40),
   assertUPBDD(X20,2,1-1,1-X40),
   assertUPBDD(LEFT,1,X21,X20),
   %right
   addSet([1-1,5-1],X15),
   addSet([3-0],X30),
   assertUPBDD(X2-1,2,1-1,1-X30),
   RIGHT = X2-X15,
   %conjunction
   conjunction(LEFT,RIGHT,UP),
   writeL([' left ',LEFT,' right ',RIGHT,' result: ',UP]),
   printTables
   .
%% LEFT : ((X1 /\ X5) -> X3) /\ (X2 -> X4)
%% RIGHT : X1 /\ X5 /\ (X3 -> X2)
%% RESULT : X1 /\ X2 /\ X3 /\ X4 /\ X5
test2a :-
   initUPBDD,
   %left
   addSet([4-1],X41),
   addSet([5-0],X50),
   assertUPBDD(X3-1,3,1-1,1-X50),
   assertUPBDD(X21,2,X3-X41,X3-1),
   assertUPBDD(X20,2,1-1,1-X41),
   assertUPBDD(LEFT,1,X21,X20),
   %right
   addSet([1-1,5-1],X15),
   addSet([3-0],X30),
   assertUPBDD(X2-1,2,1-1,1-X30),
   RIGHT = X2-X15,
   %conjunction
   conjunction(LEFT,RIGHT,UP),
   writeL([' left ',LEFT,' right ',RIGHT,' result: ',UP]),
   printTables
   .

%% Left :(X1 \/ X2) /\ ((X3 \/ X4) -> X5)
%% RIGHT :(X3 \/ X4) /\ ((X1 \/ X2) -> -X5)
%% RESULT : 0-0
test3 :-
   initUPBDD,
   %left
   addSet([5-1],X51),
   addSet([2-1],X21),
   assertUPBDD(X4-1,4,1-X51,1-1),
   assertUPBDD(X3-1,3,1-X51,X4-1),
   assertUPBDD(LEFT,1,X3-1,X3-X21),
   %right
   addSet([5-0],X50),
   addSet([4-1],X41),
   assertUPBDD(X3R-1,3,1-1,1-X41),
   assertUPBDD(X2R-1,2,X3R-X50,X3R-1),
   assertUPBDD(RIGHT,1,X3R-X50,X2R-1),
   %conjunction
   conjunction(LEFT,RIGHT,UP),
   writeL([' left ',LEFT,' right ',RIGHT,' result: ',UP]),
   printTables
   .

%% Left X1, Right X2 result X1 \/ X2
test4 :-
        initUPBDD,
        addSet([1-1],X11),
        addSet([2-1],X21),
        LEFT = 1-X11,
        RIGHT = 1-X21,
        disjunction(LEFT,RIGHT,UP),
        writeL([' left ',LEFT,' right ',RIGHT,' result: ',UP]),
        printTables
    .

%LEFT X1 \/ X2
%RIGHT (X3 \/ X4) /\ ((X1 \/ X2) -> -X5)
test5 :-
        initUPBDD,
        addSet([1-1],X11),
        addSet([2-1],X21),
        disjunction(1-X11,1-X21,LEFT),
        addSet([5-0],X50),
        addSet([4-1],X41),
        assertUPBDD(X3R-1,3,1-1,1-X41),
        assertUPBDD(X2R-1,2,X3R-X50,X3R-1),
        assertUPBDD(RIGHT,1,X3R-X50,X2R-1),
        conjunction(LEFT,RIGHT,UP),
        writeL([' left ',LEFT,' right ',RIGHT,' result: ',UP]),
        printTables
    .



%% Left X1 \/ X2
%% Right X1
%% Result X1 \/ X2
test6 :-
        initUPBDD,
        addSet([2-1],X21),
        assertUPBDD(LEFT,1,1-1,1-X21),
        addSet([1-1],X11),
        RIGHT = 1-X11,
        disjunction(LEFT,RIGHT,UP),
        writeL([' left ',LEFT,' right ',RIGHT,' result: ',UP]),
        printTables
        .

%% Left -X1
%% Right X1
%% Result 1
test7 :-
        initUPBDD,
        addSet([1-0],X10),
        addSet([1-1],X11),
        LEFT = 1-X10,
        RIGHT = 1-X11,
        disjunction(LEFT,RIGHT,UP),
        writeL([' left ',LEFT,' right ',RIGHT,' result: ',UP]),
        printTables
        .

