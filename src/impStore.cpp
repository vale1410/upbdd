
#include "impStore.h"


ImpStore::ImpStore(unsigned long size):
     store(size)
{
    ImpP iOne = new Imp(1,0x0000,impOne); // dummy element
    store[*iOne] = impOne;
}

bool ImpStore::lookup(const Imp imp) const {
    return store.find(imp) != store.end();
}  

ImpP ImpStore::add(const Imp imp) {
    //std::cout << "imp add: " << imp.toString() << std::endl;
    if (!imp._imp.any()) {
        return imp._nextP;
    }
    
    ImpStoreT::iterator i = store.find(imp);
    ImpP result;
    if (i != store.end() ) {
        result = i->second;
    } else {
        result = new Imp(imp);
        store[imp]=result;
    }            
    return result;
}

void ImpStore::debug() {
    foreach(const ImpStoreT::value_type& i , store) {
        if (i.second != impOne) i.second->printImp();
    };
}

size_t ImpStore::size() {
    return store.size();
}

// a - b = a XOR impIntersection(a,b)
ImpP ImpStore::impSubtraction(ImpP a, ImpP b) {
    if (a == impOne) {
        return impOne;
    } else if (b == impOne ) {
        return a;
    } else if (a->_block == b->_block) {
        Imp imp = Imp(a->_block,    
                      a->getPos() ^ (a->getPos() & b->getPos()),
                      a->getNeg() ^ (a->getNeg() & b->getNeg()),
                      impOne);
        imp._nextP = impSubtraction(a->_nextP,b->_nextP);
        return add(imp);
    } else if (a->_block < b->_block) {
        return impSubtraction(a,b->_nextP);
    } else {
        return impSubtraction(a->_nextP,b);
    }
}

ImpP ImpStore::impIntersection(ImpP a, ImpP b) {
    if (a == impOne || b == impOne ) {
        return impOne;
    } else if (a->_block == b->_block) {
        Imp imp = Imp(a->_block,
                      a->getPos() & b->getPos(),
                      a->getNeg() & b->getNeg(),
                      impOne);
        imp._nextP = impIntersection(a->_nextP,b->_nextP);
        return add(imp);
    } else if (a->_block < b->_block) {
        return impIntersection(a,b->_nextP);
    } else {
        return impIntersection(a->_nextP,b);
    }
}


ImpReturn ImpStore::impUnion(ImpP a, ImpP b) {
    ImpReturn result;
    if (a == impOne) {
        result.first = SAT;
        result.second = b;
        return result;
    }
    else if (b == impOne) {
        result.first = SAT;
        result.second = a;
        return result;
    }

    Imp imp;
    ImpP nextA;
    ImpP nextB; 
    
    if (a->_block == b->_block) {
        imp = Imp(a->_block,
                  a->getPos() | b->getPos(),
                  a->getNeg() | b->getNeg(),
                  impOne);
        if((imp.getPos() & imp.getNeg()).any()) {
            result.first = UNSAT;
            return result;
        } else {
            nextA = a->_nextP;
            nextB = b->_nextP;
        }
    } else if (a->_block < b->_block) {
        nextA = a;
        nextB = b->_nextP;
        imp._block = b->_block;
        imp._imp = b->_imp;
    } else {
        nextA = a->_nextP;
        nextB = b;
        imp._block = a->_block;
        imp._imp = a->_imp; 
    }
    
    ImpReturn recursion = impUnion(nextA,nextB);

    if (recursion.first == UNSAT) {
        result = recursion;
    } else {
        result.first = SAT;
        imp._nextP = recursion.second;
        result.second = add(imp);
    }
    return result;
}

ImpReturn ImpStore::impUnion(ImpP a, ImpP b, ImpP c) {
    ImpReturn result = impUnion(a,b);
    if (result.first == SAT) {
        result = impUnion(result.second,c);
    }
    return result;
}

/*
 * returns 0 if level is not in first block of imp
 * precondition: impP->_block == level/8 + 1
 *
 */
bool ImpStore::impliedLevel(ImpP impP, Level level) {
    if (impP == impOne) {
        return false;
    } else {
        return impP->getPosImp(level) || impP->getNegImp(level);
    }
}

ImpP ImpStore::adjustLevel(ImpP impP, Level level) const {
    if (impP == impOne) {
        return impP;
    } else {
        return impP->adjustLevel(level);
    }
}
        
/*
 * precondition: impP->_block <= level/8 + 1
 *
 */
ImpP ImpStore::makeNewImpWithLevel(ImpP impP, Level level, bool direction) {
    Imp newImp(0,0x0000,impOne);
    if (impP != impOne) {
        if (impP->_block == level/8 + 1) {
            newImp = *impP;
        } else {
            newImp._block = level/8 + 1;
            newImp._nextP = impP;
        }
    } else {
        newImp._block = level/8 + 1;
        newImp._nextP = impOne;
    }
    if (direction) {
        newImp.setPosImp(level);
    } else {
        newImp.setNegImp(level);
    }
    return add(newImp);
}

ImpP ImpStore::makeNewImp(ImpP impP, Variable var) {
    if (var > 0) {
        return makeNewImpWithLevel(impP,var,true);
    } else {
        return makeNewImpWithLevel(impP,-1*var,false);
    }
}

ImpP ImpStore::makeVar(Variable var) {
    return makeNewImp(impOne, var);
}

