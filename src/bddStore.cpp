#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/foreach.hpp>

#include "common.h"
#include "imp.h"
#include "bdd.h"
#include "impStore.h"
#include "bddStore.h"

// BddStore

BddStore::BddStore(unsigned long size):
     store(size)
{
    BddP bOne = new Bdd(1,bddOne,bddOne,impOne,impOne);
    store[*bOne] = bddOne;
}

bool BddStore::lookup(const Bdd bdd) const {
    return store.find(bdd) != store.end();
}  

UpBdd BddStore::add(const Bdd bdd, ImpStore& impStore)  {
    UpBdd high = bdd._high;
    UpBdd low = bdd._low;
    if (high == low) {
        return high;
    }
    UpBdd result(impOne,bddOne);
    result._impP = impStore.impIntersection(high._impP,low._impP);
    high._impP = impStore.impSubtraction(high._impP,result._impP);
    low._impP = impStore.impSubtraction(low._impP,result._impP); 
    Bdd resBdd(bdd._level,high,low);
    BddStoreT::iterator i = store.find(resBdd);
    if (i != store.end() ) {
        result._bddP = i->second;
    } else {
        result._bddP = new Bdd(resBdd); 
        store[resBdd] = result._bddP;
    }
    return result;
}

void BddStore::debug() const {
    BOOST_FOREACH(const BddStoreT::value_type& i , store) {
        if (i.second != bddOne) i.second->printBdd();
    };
}

size_t BddStore::size() {
    return store.size();
}

// Backend

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

//TODO: not implemented yet exception
bool Backend::impliedLevel(ImpP imp, Level level) {
    return false;
}

        //TODO: not implemented yet exception
ImpP Backend::adjustLevel(ImpP imp, Level level) {
    return imp;
}

        //TODO: not implemented yet exception
Level Backend::maxLevel(BddP a,BddP b) {
    if (a->_level > b->_level) {
        return a->_level;
    } else {
        return b->_level;
    }    
} 


BddReturn Backend::bddAnd(UpBdd a, UpBdd b, ImpP impP) {
    BddReturn result(SAT,UpBdd(impOne,bddOne));
    ImpReturn impR = impU// do in one step, saves some imp allocationsnion(a._impP,b._impP,impP); // do in one step, saves some imp allocations

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
            impP = adjustLevel(impP,level); // might change impP

            // check for cache

            if (impliedLevel(impP,level)) { // does the impP imply this level?
                result = bddAndCall(bddPa,bddPb,impP,impP->getPos(level));
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
                    highReturn.second._impP = highReturn.second._impP->setPos(level,true); // todo!
                    result = highReturn;
                } else if (highReturn.first == UNSAT && lowReturn.first == SAT) {
                    //allocate a new imp
                    lowReturn.second._impP = lowReturn.second._impP->setNeg(level,true); // todo!
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

