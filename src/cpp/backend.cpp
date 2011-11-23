#include <sstream>

#include "common.h"
#include "imp.h"
#include "bdd.h"
#include "impStore.h"
#include "bddStore.h"
#include "backend.h"

Backend::Backend(unsigned int bddSize, unsigned int impSize):
    bddStore(bddSize),
    impStore(impSize)
{
}

UpBdd Backend::add(const Bdd bdd) {
    return bddStore.add(bdd,impStore); 
}

ImpP Backend::add(const Imp imp) {
    return impStore.add(imp);
}

ImpReturn Backend::impUnion(ImpP a, ImpP b) {
    return impStore.impUnion(a,b);
}

ImpReturn Backend::impUnion(ImpP a, ImpP b, ImpP c) {
    return impStore.impUnion(a,b,c);
}

ImpP Backend::impIntersection(ImpP a, ImpP b) {
    return impStore.impIntersection(a,b);
}

ImpP Backend::impSubtraction(ImpP a, ImpP b) {
    return impStore.impSubtraction(a,b);
}

void Backend::debug() {
    impStore.debug();
    bddStore.debug();
}

size_t Backend::sizeImp() {
    return impStore.size();
}

size_t Backend::sizeBdd() {
    return bddStore.size();
}

void Backend::printSize() {
    std::cout << "sizeImp: " << sizeImp() << ", sizeBdd: " << sizeBdd() << std::endl;
}


/*
 * one of a and b is not bddOne
 */

Level Backend::maxLevel(BddP a,BddP b) {
    if (a == bddOne) {
        return b->_level;
    } else if (b == bddOne) {
        return a->_level;
    } else if (a->_level > b->_level) {
        return a->_level;
    } else {
        return b->_level;
    }    
} 

BddReturn Backend::bddAnd(std::vector<UpBdd> ups) {
    BddReturn tmp(SAT,UpBdd());
    foreach(const UpBdd up, ups) {
        if (tmp.first == SAT) {
            tmp = bddAnd(tmp.second,up);
        }
    };
    return tmp; }

BddReturn Backend::bddAnd(UpBdd a, UpBdd b) {
    return bddAnd(a, b, impOne);
}

BddReturn Backend::bddAnd(UpBdd a, UpBdd b, ImpP impP) {
    // check cache
    BddReturn result(SAT,UpBdd(impOne,bddOne));
    ImpReturn impAB = impUnion(a._impP,b._impP);
    if (impAB.first == UNSAT) {
        result.first = UNSAT;
    } else {
        ImpReturn impR = impUnion(impAB.second,impP);
        if (impR.first == UNSAT) {
            result.first = UNSAT;
        } else {
            ImpP impP = impR.second;
            ImpP impL = impAB.second;
            const BddP bddPa = a._bddP;
            const BddP bddPb = b._bddP;
            if (bddPa == bddOne && bddPb == bddOne ){
                result.first = SAT;
                result.second = UpBdd(impL,bddOne);        
            } else {
                const Level level = maxLevel(bddPa,bddPb);
                impP = impStore.adjustLevel(impP,level);
                if (impStore.impliedLevel(impP,level) ) { 
                    result = bddAndCall(bddPa,bddPb,impP,impP->getPosImp(level));
                    result.second._impP = impUnion(result.second._impP, impL).second;
                } else {
                    BddReturn highReturn = bddAndCall(bddPa,bddPb,impP,true);
                    BddReturn lowReturn  = bddAndCall(bddPa,bddPb,impP,false);
                    if (highReturn.first == SAT && lowReturn.first == SAT) {
                        result.first = SAT; 
                        Bdd bdd(level, highReturn.second, lowReturn.second); 
                        UpBdd up = add(bdd);
                        up._impP = impUnion(impL,up._impP).second;
                        result.second = up;
                    } else if (highReturn.first == UNSAT && lowReturn.first == UNSAT) {
                        result.first = UNSAT;
                    } else if (highReturn.first == SAT && lowReturn.first == UNSAT) {
                        highReturn.second._impP = impStore.makeNewImpWithLevel(highReturn.second._impP, level, true);  
                        highReturn.second._impP = impUnion(highReturn.second._impP, impL).second;
                        result = highReturn;
                    } else if (highReturn.first == UNSAT && lowReturn.first == SAT) {
                        lowReturn.second._impP = impStore.makeNewImpWithLevel(lowReturn.second._impP, level, false);  
                        lowReturn.second._impP = impUnion(lowReturn.second._impP, impL).second;
                        result = lowReturn;
                    }  
                }
            }
        }    
    }
    // make result persistent
    return result;
}


// TODO: refactoring: make global impOne and bddOne with level 0
// this way 2 less if#s
BddReturn Backend::bddAndCall(const BddP a, const BddP b, const ImpP impP, const bool direction) {
    UpBdd aUpBdd(impOne,a);
    UpBdd bUpBdd(impOne,b);
    if (b == bddOne) {
        if (direction) {
            return bddAnd(a->_high, bUpBdd, impP);
        } else {
            return bddAnd(a->_low, bUpBdd, impP);
        }
    } else if (a == bddOne) {
        if (direction) {
            return bddAnd(aUpBdd, b->_high, impP);
        } else {
            return bddAnd(aUpBdd, b->_low, impP);
        }
    } else if (a->_level > b->_level) {
        if (direction) {
            return bddAnd(a->_high, bUpBdd, impP);
        } else {
            return bddAnd(a->_low, bUpBdd, impP);
        }
    } else if (a->_level < b->_level) {
        if (direction) {
            return bddAnd(aUpBdd, b->_high, impP);
        } else {
            return bddAnd(aUpBdd, b->_low, impP);
        }
    } else {
        if (direction) {
            return bddAnd(a->_high, b->_high, impP);
        } else {
            return bddAnd(a->_low, b->_low, impP);
        }
    } 
}

//BddReturn Backend::bddImply(UpBdd up, ImpP impP) {
//    BddReturn result(SAT,up);
//    if (impP = impOne || bddP == bddOne) {
//        return result;
//    } else  {
//        impP = impStore.adjustLevel(impP,level);
//        BddP bddP = up._bddP;
//        ImpP impPP = up._impP;
//        ImpReturn impR = impUnion(impPP,impP); // can be done faster, without actuall making the result persistent
//        if (impR.first == UNSAT) {
//            result.first = UNSAT;
//        } else if (impStore.impliedLevel(impP,bddP->_level)) {
//            if (impP->getPosImp(bddP->_level)) {
//                result = bddImply(bddP->_high,impP);
//            } else {
//                result = bddImply(bddP->_low,impP);
//            }
//            if (result.first == SAT) {
//                result.second = impUnion(result.second,impPP); // can be done faster
//                result.second = impSubtraction(result.second,impP); // can be done faster
//            }
//        } else {


UpBdd Backend::makeClause(Clause clause) {
    UpBdd up;
    Clause::iterator iter = clause.begin();
    if(iter != clause.end()) {
        const UpBdd upOne;
        Bdd bdd;
        ImpP impP = impStore.makeVar(*iter);
        iter++;
        up = UpBdd(impP,bddOne);
        for(;iter != clause.end();iter++) {
            if (*iter > 0 ) {
                bdd = Bdd(*iter,upOne,up);
                up = add(bdd);
            } else {
                bdd = Bdd(-1*(*iter),up,upOne);
                up = add(bdd);
            }
        };
    }
    return up;
}

std::vector<UpBdd> Backend::makeClauses(RawProblem problem) {
    std::vector<UpBdd> result(problem.size());
    int i = 0;
    foreach(const Clause clause, problem) {
        result[i] = makeClause(clause);
        i++;
    };
    return result;
}

ImpP Backend::makeImplication(RawImplication implication) {
    ImpP impP = impOne;
    foreach(const Literal var, implication) {
        impP = impStore.makeNewImp(impP,var);
    };
    return impP;
}

void Backend::printClause(Clause clause) {
    for(Clause::iterator iter = clause.begin(); iter!=clause.end(); iter++) {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;
}
