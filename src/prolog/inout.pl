:-module(inout,[getConstraints/2]).
:-use_module(backbone,[setLevels/1]).
:-use_module(utils).


getConstraints(ListOfConstraints,File):-
   see(File),
   read(file(Name)),
   read(nbvar(NVariables)),
   read(nbconstr(NConstraints)),
   read(variableOrdering(_VariableOrdering)),
   getAllConstraints(ListOfConstraints,File),
   seen,write(Name),write('\t'),write(NVariables),write('\t'),write(NConstraints),
   %writeLTable([Name ,'\t',NVariables,'\t',NConstraints ]),
   %write([Name ,'\t',NVariables,'\t',NConstraints ]),
   setLevels(NVariables).

getAllConstraints([constraint(CSorted,T)|R],File):-
   read(constraint(C,T)),
   sort(C,CSorted),
   !,
   getAllConstraints(R,File).
   
getAllConstraints([],_File).




