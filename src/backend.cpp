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


Level Backend::maxLevel(BddP a,BddP b) {
    if (a->_level > b->_level) {
        return a->_level;
    } else {
        return b->_level;
    }    
} 


BddReturn Backend::bddAnd(UpBdd a, UpBdd b, ImpP impP) {
    BddReturn result(SAT,UpBdd(impOne,bddOne));
    ImpReturn impR = impUnion(a._impP,b._impP,impP);

    if (impR.first == UNSAT) {
        result.first = UNSAT;
    } else {
        ImpP impP = impR.second;
        const BddP bddPa = a._bddP;
        const BddP bddPb = b._bddP;
        if (bddPa == bddOne && bddPb == bddOne ){
            result.first = SAT;
            result.second = UpBdd(impP,bddOne);        
        } else {
            const Level level = maxLevel(bddPa,bddPb);
            impP = impStore.adjustLevel(impP,level); // might change impP

            if (impStore.impliedLevel(impP,level) ) { // does the impP imply this level?
                return bddAndCall(bddPa,bddPb,impP,impP->getPos(level));
            } else {
                BddReturn highReturn = bddAndCall(bddPa,bddPb,impP,true);
                BddReturn lowReturn  = bddAndCall(bddPa,bddPb,impP,false);
                if (highReturn.first == SAT && lowReturn.first == SAT) {
                    result.first = SAT;
                    Bdd bdd(level, highReturn.second, lowReturn.second);
                    result.second = add(bdd);
                } else if (highReturn.first == UNSAT && lowReturn.first == UNSAT) {
                    result.first = UNSAT;
                } else if (highReturn.first == SAT && lowReturn.first == UNSAT) {
                    //allocate a new imp
                    highReturn.second._impP = impStore.makeNewImpWithLevel(highReturn.second._impP, level, true);  
                    result = highReturn;
                } else if (highReturn.first == UNSAT && lowReturn.first == SAT) {
                    //allocate a new imp
                    lowReturn.second._impP = impStore.makeNewImpWithLevel(highReturn.second._impP, level, false);  
                    result = lowReturn;
                }  
            }
        }
    }    
    return result;
}

BddReturn Backend::bddAndCall(const BddP a, const BddP b, const ImpP impP, const bool direction) {
    UpBdd aUpBdd(impOne,a);
    UpBdd bUpBdd(impOne,b);
    if (a->_level == b->_level) {
        if (direction) {
            return bddAnd(a->_high, b->_high, impP);
        } else {
            return bddAnd(a->_low, b->_low, impP);
        }
    } else if (a->_level > b->_level) {
        if (direction) {
            return bddAnd(a->_high, bUpBdd, impP);
        } else {
            return bddAnd(a->_low, bUpBdd, impP);
        }
    } else {
        if (direction) {
            return bddAnd(aUpBdd, b->_high, impP);
        } else {
            return bddAnd(aUpBdd, b->_low, impP);
        }
    }
}

