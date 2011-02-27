#ifndef common_h
#define common_h

class Imp;
class Bdd;
class UpBdd;

typedef unsigned int Level;

typedef Bdd* BddP;
typedef Imp* ImpP;

enum Feedback { SAT, UNSAT, BREAK };

typedef std::pair<Feedback,ImpP> ImpReturn;
typedef std::pair<Feedback,UpBdd> BddReturn;

BddP bddOne = NULL;
ImpP impOne = NULL;

#endif
