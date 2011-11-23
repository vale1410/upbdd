%example from my paper
file('t3').
nbvar(4).
nbconstr(3).
variableOrdering([[0,0],[1,1],[2,2],[3,3]]).
constraint([[0,1,1],[3,1,0]],1).
constraint([[1,1,0],[2,1,0]],1).
constraint([[1,1,1],[2,1,1]],1).

