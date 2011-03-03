#ifndef common_h
#define common_h

#include <vector>

#define impOne NULL
#define bddOne NULL
#define BLOCK 8
#define foreach BOOST_FOREACH

typedef unsigned int Level;
typedef unsigned int Block;
typedef int Variable;

typedef std::vector<Variable> Clause;

enum Feedback { SAT, UNSAT, BREAK };

#endif
