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
class ImpHashFunction;
class BddHashFunction;

enum Feedback { SAT, UNSAT, BREAK };

typedef unsigned int Level;
typedef Imp* ImpP;
typedef Bdd* BddP;
typedef std::pair<ImpP,BddP> UpBdd;
typedef std::pair<Feedback,ImpP> ImpReturn;
typedef std::pair<Feedback,UpBdd> BddReturn;


class Imp {
    public:


    Imp(Level level, std::bitset<2*8> imp, Imp* next):
        _level(level),
        _imp(imp),
        _next(next)
    {}

    Level _level;
    std::bitset<2*8> _imp;
    Imp* _next;
    
    std::string toString() const {
        std::ostringstream oss;
        oss << this << " = " << "(" << _level <<  "," << pos() <<  "," << neg() << "," << _next <<  ")";
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
};


class Bdd {
    public:
        
    Bdd(Level level, BddP high, BddP low, ImpP hImp, ImpP lImp) :
        _level(level),
        _high(high),
        _low(low),
        _hImp(hImp),
        _lImp(lImp) 
        {}

     Level _level;
     BddP _high;
     BddP _low;
     ImpP _hImp;
     ImpP _lImp;
     
     std::string toString() const {
         std::ostringstream oss;
         oss << this << " = " << "(" << _level <<  "," << _high <<  "," << _low <<  "," << _hImp <<  "," << _lImp <<  ")";
         return oss.str();
     }
};

Bdd* bddOne = NULL;
Imp* impOne = NULL;

class ImpHashFunction : std::unary_function< Imp , size_t > {
    public:
        inline size_t operator() (const Imp& imp) const {
            boost::hash<ImpP> hasher;
            std::size_t hash = imp._imp.to_ulong() + hasher(imp._next) + imp._level;
            boost::hash_combine(hash, imp._imp.to_ulong());
            boost::hash_combine(hash, imp._next);
            boost::hash_combine(hash, imp._level);
            return hash;
        }
};

class BddHashFunction : std::unary_function< Bdd , size_t > {
    public:
        inline size_t operator() (const Bdd& bdd) const {
            boost::hash<BddP> hasherBdd;
            boost::hash<ImpP> hasherImp;
            std::size_t hash = hasherBdd(bdd._high) + hasherBdd(bdd._low) + hasherImp(bdd._hImp) + hasherImp(bdd._lImp) + bdd._level;
            boost::hash_combine(hash, bdd._high);
            boost::hash_combine(hash, bdd._low);
            boost::hash_combine(hash, bdd._hImp);
            boost::hash_combine(hash, bdd._lImp);
            boost::hash_combine(hash, bdd._level);
            return hash;
        }
};

class BddEqual : std::equal_to< Bdd > {
    public:
        bool operator() (const Bdd& a, const Bdd& b) const {
            return (a._level ==  b._level &&
                    a._high  ==  b._high  &&
                    a._low   ==  b._low   &&
                    a._hImp  ==  b._hImp  &&
                    a._lImp  ==  b._lImp);
        }
};


class ImpEqual : std::equal_to< Imp > {
    public:
        bool operator() (const Imp& a, const Imp& b) const {
            return (a._level ==  b._level &&
                    a._imp   ==  b._imp  &&
                    a._next  ==  b._next);
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

        BddP add(const Bdd bdd)  {
            BddStoreT::iterator i = store.find(bdd);
            BddP result;
            if (i != store.end() ) {
                result = i->second;
            } else {
                result = new Bdd(bdd);
                store[bdd]=result;
            }            
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
            return bddStore.add(bdd);
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
};

ImpReturn impAnd(ImpP a, ImpP b, Backend::SP backend) {
    ImpReturn result;
    Imp imp(0,0x00,NULL);
    Imp* nextA;
    Imp* nextB;
    ImpReturn recursion;
    if (a->_level == b->_level) {
        imp._level = a->_level;
        imp.setPos(a->getPos() | b->getPos());
        imp.setNeg(a->getNeg() | b->getNeg());
        if(imp.getPos().to_ulong() & imp.getNeg().to_ulong()) {
            result.first = UNSAT;
            return result;
        } else {
            nextA = a->_next;
            nextB = b->_next;
        }
    } else if (a->_level < b->_level) {
        nextA = a;
        nextB = b->_next;
        imp._level = b->_level;
        imp._imp = b->_imp;
    } else {
        nextA = a->_next;
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
        recursion = impAnd(nextA,nextB,backend);
    }

    if (recursion.first == UNSAT) {
        return recursion;
    } else {
        result.first = SAT;
        imp._next = recursion.second;
        result.second = backend->add(imp);
        return result;
    }
    return result;
}

BddReturn bddAnd(BddP a, BddP b, ImpP imp, Backend::SP backend) {
    BddReturn result;
    // check in cache wheather a,b has been called before
    if (imp != impOne) {
        //check if implication has effects on a or b
        // if yes, go further down and call again bddAnd
    }
    if (a->_level == b->_level) {
        
    }
    return result;
}


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

int main () 
{
    testSizes();
    testImp();
    testBdd();
} 

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

void testBdd() {
    std::cout << "testBdd 1: " << testBdd1() << std::endl;
    std::cout << "testBdd 2: " << testBdd2() << std::endl;
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
    ImpReturn result = impAnd(iP1,iP2,backend);
    ok = result.first == SAT;
    ok = result.second->_imp == i3._imp;
    ok = before + 3 == backend->sizeImp();
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
    ImpReturn result = impAnd(iP1,iP2,backend);
    ok = result.first == UNSAT;
    ok = before + 2 == backend->sizeImp();
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
    ImpReturn result = impAnd(iP1,iP2,backend);
    ok = result.first == SAT;
    ok = before + 3 == backend->sizeImp();
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
    ImpReturn result = impAnd(iP2,iP1,backend);
    ok = result.first == SAT;
    ok = before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}

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
    std::cout << "output of bddOne " << bddOne << std::endl;
    std::cout << "output of impOne " << impOne << std::endl;
}
