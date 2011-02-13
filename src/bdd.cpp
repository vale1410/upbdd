#include <iostream>
#include <string>
#include <sstream>
#include <bitset>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/foreach.hpp>

class Imp;
class Bdd;
class UpBdd;
class ImpHashFunction;
class BddHashFunction;

enum Feedback { SAT, UNSAT, BREAK };

typedef unsigned int Level;
typedef Imp* ImpP;
typedef Bdd* BddP;
typedef std::pair<Feedback,ImpP> ImpReturn;
typedef std::pair<Feedback,UpBdd> BddReturn;

Bdd* bddOne = NULL;
Imp* impOne = NULL;

class Imp {
    public:


    Imp(Level level, std::bitset<2*8> imp, ImpP next):
        _level(level),
        _imp(imp),
        _nextP(next)
    {}

    Level _level;
    std::bitset<2*8> _imp;
    ImpP _nextP;
    
    std::string toString() const {
        std::ostringstream oss;
        oss << this << " = " << "(" << _level <<  "," << pos() <<  "," << neg() << "," << _nextP <<  ")";
        return oss.str();
    }

    void setPos(std::bitset<8> bits) {
        for(size_t i= 0 ; i<_imp.size()/2;i++) {
            setPos(i,bits[7-i]);
        }
    }

    void setNeg(std::bitset<8> bits) {
        for(size_t i= 0 ; i<_imp.size()/2;i++) {
            setNeg(i,bits[7-i]);
        }
    }
    
    std::bitset<8> getPos() const {
        std::bitset<8> result;
        for(size_t i= 0 ; i<_imp.size()/2;i++) {
            result[7-i] = getPos(i);
        }
        return result;
    }

    std::bitset<8> getNeg() const {
        std::bitset<8> result;
        for(size_t i= 0 ; i<_imp.size()/2;i++) {
            result[7-i] = getNeg(i);
        }
        return result;
    }
    
    bool getPos(size_t i) const {
        return _imp.test(_imp.size()-1-i);
    }

    bool getNeg(size_t i) const {
        return _imp.test(_imp.size()/2-1-i);
    }
    
    void setPos(size_t i,bool b) {
        _imp[_imp.size()-1-i] = b;
    }

    void setNeg(size_t i, bool b) {
        _imp[_imp.size()/2-1-i] = b;
    }

    std::string pos() const {
        std::ostringstream oss;
        oss << getPos();
        return oss.str();
    }

    std::string neg() const {
        std::ostringstream oss;
        oss << getNeg();
        return oss.str();
    }
     /*
    BddP apply(BddP bdd) {
                    
    }
    */
};


class UpBdd {
    public:

    UpBdd(ImpP impP,BddP bddP):
        _bddP(bddP),
        _impP(impP)
    {} 

    bool operator==(const UpBdd &other) const {
        return _bddP == other._bddP && _impP == other._impP;
    }


    BddP _bddP;
    ImpP _impP;
};

class Bdd {
    public:
        
    Bdd(Level level, UpBdd high, UpBdd low) :
        _level(level),
        _high(high),
        _low(low)
        {}
                
    Bdd(Level level, BddP highP, BddP lowP, ImpP hImpP, ImpP lImpP) :
        _level(level),
        _high(hImpP,highP),
        _low(lImpP,lowP)
        {}

    Level _level;
    UpBdd _high; 
    UpBdd _low;
    
    std::string toString() const {
        std::ostringstream oss;
        // << this << " = " << "(" << _level <<  ",<" << _high._impP <<  "," << _low._bddP <<  ">,<" << _low._impP <<  "," << _low._bddP <<  ">)";
        return oss.str();
    }
};


class ImpHashFunction : std::unary_function< Imp , size_t > {
    public:
        inline size_t operator() (const Imp& imp) const {
            boost::hash<ImpP> hasher;
            std::size_t hash = imp._imp.to_ulong() + hasher(imp._nextP) + imp._level;
            boost::hash_combine(hash, imp._imp.to_ulong());
            boost::hash_combine(hash, imp._nextP);
            boost::hash_combine(hash, imp._level);
            return hash;
        }
};

class BddHashFunction : std::unary_function< Bdd , size_t > {
    public:
        inline size_t operator() (const Bdd& bdd) const {
            boost::hash<BddP> hasherBdd;
            boost::hash<ImpP> hasherImp;
            std::size_t hash = hasherBdd(bdd._high._bddP) + hasherBdd(bdd._low._bddP) + hasherImp(bdd._high._impP) + hasherImp(bdd._low._impP) + bdd._level;
            boost::hash_combine(hash, bdd._high._bddP);
            boost::hash_combine(hash, bdd._low._bddP);
            boost::hash_combine(hash, bdd._high._impP);
            boost::hash_combine(hash, bdd._low._impP);
            boost::hash_combine(hash, bdd._level);
            return hash;
        }
};

class UpBddEqual : std::equal_to< UpBdd > {
    public:
        bool operator() (const UpBdd& a, const UpBdd& b) const {
            return (a._bddP ==  b._bddP &&
                    a._impP  ==  b._impP);
        }
};

class BddEqual : std::equal_to< Bdd > {
    public:
        bool operator() (const Bdd& a, const Bdd& b) const {
            return (a._level ==  b._level &&
                    a._high  ==  b._high  &&
                    a._low   ==  b._low);
        }
};


class ImpEqual : std::equal_to< Imp > {
    public:
        bool operator() (const Imp& a, const Imp& b) const {
            return (a._level ==  b._level &&
                    a._imp   ==  b._imp  &&
                    a._nextP  ==  b._nextP);
        }
};

void printImp(const Imp* imp)
{
    ImpHashFunction hasher;
    std::cout << "imp: " << imp->toString() << "\t hash: " << hasher(*imp) << std::endl;
}

void printBdd(const Bdd* bdd)
{
    BddHashFunction hasher;
    std::cout << "bdd: " << bdd->toString() << "\t hash: " << hasher(*bdd) << std::endl;
}

typedef boost::unordered_map< Bdd, BddP, BddHashFunction, BddEqual > BddStoreT;
typedef boost::unordered_map< Imp, ImpP, ImpHashFunction, ImpEqual > ImpStoreT;


class IntersectResult {
    public: 
        
        ImpP _upP;
        ImpP _highP;
        ImpP _lowP;
};


class BddStore {
    public: 

        BddStore(unsigned long size):
             store(size)
        {
            BddP bOne = new Bdd(1,bddOne,bddOne,impOne,impOne);
            store[*bOne] = bddOne;
        }

        bool lookup(const Bdd bdd) const {
            return store.find(bdd) != store.end();
        }  

        
        UpBdd add(const Bdd bdd)  {
            UpBdd high = bdd._high;
            UpBdd low = bdd._low;
            UpBdd result(impOne,bddOne);
            /*
            if (high == low) {
                return high;
            }

            IntersectResult intersection = intersection(high._impP,low._impP);
            high._impP = intersection._highP;
            low._impP = intersection._lowP;
            //Bdd resBdd(
            BddStoreT::iterator i = store.find(bdd);
            if (i != store.end() ) {
                result = UpBdd(impOne,i->second);
            } else if(bdd._high == bdd._low && bdd._hImp == bdd._lImp) {
                result = new Bdd(bdd); // memory unsafe!
                store[bdd]=result;
            }*/
            return result;
        }
        
        void debug() const {
            BOOST_FOREACH(const BddStoreT::value_type& i , store) {
                if (i.second != bddOne) printBdd(i.second);
            };
        }
        
        size_t size() {
            return store.size();
        }
    private:
        BddStoreT store;
};

class ImpStore {
    public: 

        ImpStore(unsigned long size):
             store(size)
        {
            ImpP iOne = new Imp(1,0x0000,impOne);
            store[*iOne] = impOne;
        }

        bool lookup(const Imp bdd) const {
            return store.find(bdd) != store.end();
        }  

        ImpP add(const Imp imp) {
            //std::cout << "imp add: " << imp.toString() << std::endl;
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

        void debug() {
            BOOST_FOREACH(const ImpStoreT::value_type& i , store) {
                if (i.second != impOne) printImp(i.second);
            };
        }

        size_t size() {
            return store.size();
        }

    private:
        ImpStoreT store;
};

class Backend {

    public:
    
        typedef boost::shared_ptr<Backend> SP;
        typedef const boost::shared_ptr<Backend> ConstSP;

        Backend(unsigned int bddSize, unsigned int impSize):
            bddStore(bddSize),
            impStore(impSize)
        {
        }

        inline bool lookup(const Bdd bdd) const {
            return bddStore.lookup(bdd);
        }

        inline BddP add(const Bdd bdd) {
            return bddStore.add(bdd)._bddP; // TODO correct
        }

        inline bool lookup(const Imp imp) const {
            return impStore.lookup(imp);
        }

        inline ImpP add(const Imp imp) {
            return impStore.add(imp);
        }

        void debug() {
            impStore.debug();
            bddStore.debug();
        }

        size_t sizeImp() {
            return impStore.size();
        }

        size_t sizeBdd() {
            return bddStore.size();
        }

    private:
        BddStore bddStore;
        ImpStore impStore;
        //BddAndStore bddAndStore;  // cache for andBdd
};


ImpReturn impUnion(ImpP a, ImpP b, Backend::SP backend) {
    ImpReturn result;
    Imp imp(0,0x00,NULL);
    ImpP nextA;
    ImpP nextB;
    ImpReturn recursion;
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
    
    if (nextA == impOne) {
        recursion.first = SAT;
        recursion.second = nextB;
    }
    else if (nextB == impOne) {
        recursion.first = SAT;
        recursion.second = nextA;
    } else {
        recursion = impUnion(nextA,nextB,backend);
    }

    if (recursion.first == UNSAT) {
        return recursion;
    } else {
        result.first = SAT;
        imp._nextP = recursion.second;
        result.second = backend->add(imp);
        return result;
    }
    return result;
}

/* TODO: finish bddAnd
BddReturn bddAnd(UpBdd a, UpBdd b, ImpP imp, Backend::SP backend) {
    BddReturn result;
    ImpReturn impR = andImp(imp,a.first,b.first);
    if (impR.first == UNSAT) {
        result.first = UNSAT;
        return result; 
    } else {
        imp = impR.second;
        UpBdd aa = imp->apply(a.second);
        aa.first = impOne;
        UpBdd bb = imp->apply(b.second);
        bb.first = impOne;
        if (a.second != aa.second || b.second != bb.second) {
            return bddAnd(aa,bb,imp,backend);
        }

        if (a->_level == b->_level) {
            BddReturn hReturn = bddAnd(aa.second->getUpBddHigh(),bb.second->getUpBddHigh());    

            BddReturn lReturn = bddAnd(aa.second->getUpBddLow(),bb.second->getUpBddLow());    
        }
        return result;
}
*/


void testSetup() {
    Backend::SP backend(new Backend(20,20));
    typedef const std::bitset<16> BImp;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0001,impOne); // -1
    Imp i3(1,0x0200,impOne); // 2
    Imp i4(1,0x0002,impOne); // -2
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpP iP4 = backend->add(i4);
    Bdd b1 = Bdd(3,bddOne,bddOne,iP1,iP2);
    Bdd b2 = Bdd(3,bddOne,bddOne,iP3,iP4);
    backend->add(b1);
    backend->add(b2);
    backend->debug();
}

void testSizes();
void testImp();
void testBdd();
void testHashFunction();

int main () 
{
    //testSizes();
    testImp();
    //testBdd();
    //testHashFunction();
} 
/*
bool testBdd1() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeBdd();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0001,impOne); // -1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    Bdd bdd1(2,bddOne,bddOne,iP1,iP2); // 2 -> 1 ; -1
    Bdd bdd2(2,bddOne,bddOne,iP1,iP2); // 2 -> 1 ; -1
    backend->add(bdd1);
    backend->add(bdd2);
    return before + 1 == backend->sizeBdd();
}

bool testBdd2() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeBdd();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0001,impOne); // -1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    Bdd bdd1(2,bddOne,bddOne,iP1,iP2); // 2 -> 1 ; -1
    Bdd bdd2(2,bddOne,bddOne,iP2,iP1); // 2 -> -1 ; 1
    backend->add(bdd1);
    backend->add(bdd2);
    return before + 2 == backend->sizeBdd();
}
*/

void testBdd() {
    //std::cout << "testBdd 1: " << testBdd1() << std::endl;
    //std::cout << "testBdd 2: " << testBdd2() << std::endl;
    //std::cout << "testBdd 3: " << testBdd3() << std::endl;
    //std::cout << "testBdd 4: " << testBdd4() << std::endl;
    //std::cout << "testBdd 5: " << testBdd5() << std::endl;
    //std::cout << "testBdd 6: " << testBdd6() << std::endl;
    //std::cout << "testBdd 7: " << testBdd6() << std::endl;
}


bool testImp1() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0100,impOne); // 1
    backend->add(i1);
    backend->add(i2);
    return before + 1 == backend->sizeImp();
}

bool testImp2() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0110,impOne); // 1
    backend->add(i1);
    backend->add(i2);
    return before + 2 == backend->sizeImp();
}


bool testImp3() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0x0100,impOne); // 1
    backend->add(i1);
    backend->add(i2);
    return before + 2 == backend->sizeImp();
}


bool testImp4() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0010,impOne); // 1
    Imp i3(1,0x0110,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpReturn result = impUnion(iP1,iP2,backend);
    ok = result.first == SAT;
    ok = ok && result.second->_imp == i3._imp;
    ok = ok && before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}


bool testImp5() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0001,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpReturn result = impUnion(iP1,iP2,backend);
    ok = result.first == UNSAT;
    ok = ok && before + 2 == backend->sizeImp();
    return ok;
}


bool testImp6() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0x0001,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpReturn result = impUnion(iP1,iP2,backend);
    ok = result.first == SAT;
    ok = ok && before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}


bool testImp7() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0x0001,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpReturn result = impUnion(iP2,iP1,backend);
    ok = result.first == SAT;
    ok = ok && before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}

// impIntersection
/*
bool testImp7() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0010,impOne); // 1
    Imp i3(1,0x0110,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpReturn result = impIntersection(iP1,iP2,backend);
    ok = result.first == SAT;
    ok = ok && result.second->_imp == i3._imp;
    ok = ok && before + 3 == backend->sizeImp();
    backend->debug();
    return ok;
}
*/

void testImp() {
    std::cout << "testImp 1: " << testImp1() << std::endl;
    std::cout << "testImp 2: " << testImp2() << std::endl;
    std::cout << "testImp 3: " << testImp3() << std::endl;
    std::cout << "testImp 4: " << testImp4() << std::endl;
    std::cout << "testImp 5: " << testImp5() << std::endl;
    std::cout << "testImp 6: " << testImp6() << std::endl;
    std::cout << "testImp 7: " << testImp6() << std::endl;
}



void testSizes() {
    Level level = 123;
    std::bitset<16> bits = 0xF00F;
    Imp imp(level,bits,impOne);
    const Bdd bdd(level,bddOne,bddOne,impOne,&imp);
    int a;
    long b;
    size_t c;
    std::cout << "size of imp " << sizeof(imp) << std::endl;
    std::cout << "size of bdd " << sizeof(bdd) << std::endl;
    std::cout << "size of imp.imp " << sizeof(imp._imp) << std::endl;
    std::cout << "size of int " << sizeof(a) << std::endl;
    std::cout << "size of long " << sizeof(b) << std::endl;
    std::cout << "size of size_t " << sizeof(c) << std::endl;
    std::cout << "where points bddOne " << bddOne << std::endl;
    std::cout << "where points impOne " << impOne << std::endl;
}


void testHashFunction() {
    size_t hash = 0; 
    boost::hash_combine(hash,1);
    std::cout << hash << std::endl;
    boost::hash_combine(hash,1);
    std::cout << hash << std::endl;
    boost::hash_combine(hash,2);
    std::cout << hash << std::endl;
    boost::hash_combine(hash,3);
    std::cout << hash << std::endl;
}

