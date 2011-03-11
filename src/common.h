#ifndef common_h
#define common_h

#include <boost/foreach.hpp>
#include <vector>
#include <cmath>

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

class LiteralOrder {
    public:
        bool operator() (const Literal l1, const Literal l2) {
            return(abs(l1) < abs(l2));
        }
};

class ClauseOrder {
    public:
        bool operator() (const Clause c1, const Clause c2) {
            for(unsigned int i = 0; i < c1.size() && i < c2.size() ; i++) {
                if(abs(c1[i]) > abs(c2[i])) {
                   return false;
                } else if (abs(c1[i]) < abs(c2[i])) { 
                    break;
                }
            }
            return true;
        }
};


#endif
