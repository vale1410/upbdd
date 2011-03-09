#ifndef common_h
#define common_h

#include <boost/foreach.hpp>
#include <vector>

#define impOne NULL
#define bddOne NULL
#define BLOCK 8
#define foreach BOOST_FOREACH

typedef unsigned int Level;
typedef unsigned int Block;
typedef int Literal;

/*
 * Clause and RawImplication are correctly ordered 1,-2,-3,4 ... 
 */
typedef std::vector<Literal> RawImplication;
typedef std::vector<Literal> Clause;
typedef std::vector<Clause> RawProblem;

enum Feedback { UNSAT, SAT }; //, BREAK };

#endif
