%example from my paper
file('t7').
nbvar(6).
nbconstr(7).
variableOrdering([[0,0],[1,1],[2,2],[3,3],[4,4],[5,5]]).
constraint([[0,1,1],[3,1,1]],1).
constraint([[0,1,1],[3,1,0],[4,1,1]],1).
constraint([[0,1,1],[3,1,0],[4,1,0],[5,1,1]],1).
constraint([[0,1,0],[1,1,1],[2,1,1],[5,1,1]],1).
constraint([[0,1,0],[1,1,1],[2,1,0],[3,1,1]],1).
constraint([[0,1,0],[1,1,1],[2,1,0],[3,1,0],[4,1,1]],1).
constraint([[0,1,0],[1,1,1],[2,1,0],[3,1,0],[4,1,0],[5,1,1]],1).

