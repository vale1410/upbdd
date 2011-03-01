#include <boost/foreach.hpp>

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
    BOOST_FOREACH(const ImpStoreT::value_type& i , store) {
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
    } else if (a->_level == b->_level) {
        Imp imp = Imp(a->_level,0x0000,impOne);
        imp.setPos(a->getPos() ^ (a->getPos() & b->getPos()));
        imp.setNeg(a->getNeg() ^ (a->getNeg() & b->getNeg()));
        imp._nextP = impSubtraction(a->_nextP,b->_nextP);
        return add(imp);
    } else if (a->_level < b->_level) {
        return impSubtraction(a,b->_nextP);
    } else {
        return impSubtraction(a->_nextP,b);
    }
}

ImpP ImpStore::impIntersection(ImpP a, ImpP b) {
    if (a == impOne || b == impOne ) {
        return impOne;
    } else if (a->_level == b->_level) {
        Imp imp = Imp(1,0x0000,impOne);
        imp._level = a->_level;
        imp.setPos(a->getPos() & b->getPos());
        imp.setNeg(a->getNeg() & b->getNeg());
        imp._nextP = impIntersection(a->_nextP,b->_nextP);
        return add(imp);
    } else if (a->_level < b->_level) {
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

    Imp imp(0,0x0000,NULL);
    ImpP nextA;
    ImpP nextB; 
    
    if (a->_level == b->_level) {
        imp._level = a->_level;
        imp.setPos(a->getPos() | b->getPos());
        imp.setNeg(a->getNeg() | b->getNeg());
        if(imp.getPos().to_ulong() & imp.getNeg().to_ulong()) {
            result.first = UNSAT;
            return result;
        } else {
            nextA = a->_nextP;
            nextB = b->_nextP;
        }
    } else if (a->_level < b->_level) {
        nextA = a;
        nextB = b->_nextP;
        imp._level = b->_level;
        imp._imp = b->_imp;
    } else {
        nextA = a->_nextP;
        nextB = b;
        imp._level = a->_level;
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

//TODO: not implemented yet exception
bool ImpStore::impliedLevel(ImpP imp, Level level) {
    return false;
}

        //TODO: not implemented yet exception
ImpP ImpStore::adjustLevel(ImpP imp, Level level) {
    return imp;
}
        
//TODO: not implemented yet exception
ImpP ImpStore::makeNewImpWithLevel(ImpP imp, Level level, bool direction) {
    return imp;
}

