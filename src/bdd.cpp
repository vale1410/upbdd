#include <iostream>
#include <cstdlib>
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

BddP bddOne = NULL;
ImpP impOne = NULL;

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
        oss << this << " = " << "(" << _level <<  ",<" << _high._impP <<  "," << _high._bddP <<  ">,<" << _low._impP <<  "," << _low._bddP <<  ">)";
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

void printImp(const ImpP imp)
{
    ImpHashFunction hasher;
    std::cout << "imp: " << imp->toString() << "\t hash: " << hasher(*imp) << std::endl;
}

void printBdd(const BddP bdd)
{
    BddHashFunction hasher;
    std::cout << "bdd: " << bdd->toString() << "\t hash: " << hasher(*bdd) << std::endl;
}

typedef boost::unordered_map< Bdd, BddP, BddHashFunction, BddEqual > BddStoreT;
typedef boost::unordered_map< Imp, ImpP, ImpHashFunction, ImpEqual > ImpStoreT;

class ImpStore {
    public: 

        ImpStore(unsigned long size):
             store(size)
        {
            ImpP iOne = new Imp(1,0x0000,impOne);
            store[*iOne] = impOne;
        }

        bool lookup(const Imp imp) const {
            return store.find(imp) != store.end();
        }  

        ImpP add(const Imp imp) {
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

        void debug() {
            BOOST_FOREACH(const ImpStoreT::value_type& i , store) {
                if (i.second != impOne) printImp(i.second);
            };
        }

        size_t size() {
            return store.size();
        }

        // a - b = a XOR impIntersection(a,b)
        ImpP impSubtraction(ImpP a, ImpP b) {
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
        
        ImpP impIntersection(ImpP a, ImpP b) {
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
        

        ImpReturn impUnion(ImpP a, ImpP b) {
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

        ImpReturn impUnion(ImpP a, ImpP b, ImpP c) {
            ImpReturn result = impUnion(a,b);
            if (result.first == SAT) {
                result = impUnion(result.second,c);
            }
            return result;
        }

    private:
        ImpStoreT store;
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
        
        UpBdd add(const Bdd bdd, ImpStore& impStore)  {
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


class Backend {

    public:
    
        typedef boost::shared_ptr<Backend> SP;
        typedef const boost::shared_ptr<Backend> ConstSP;

        Backend(unsigned int bddSize, unsigned int impSize):
            bddStore(bddSize),
            impStore(impSize)
        {
        }

        UpBdd add(const Bdd bdd) {
            return bddStore.add(bdd,impStore); 
        }

        ImpP add(const Imp imp) {
            return impStore.add(imp);
        }

        ImpReturn impUnion(ImpP a, ImpP b) {
            return impStore.impUnion(a,b);
        }

        ImpP impIntersection(ImpP a, ImpP b) {
            return impStore.impIntersection(a,b);
        }

        ImpP impSubtraction(ImpP a, ImpP b) {
            return impStore.impSubtraction(a,b);
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


BddReturn bddAnd(UpBdd a, UpBdd b, ImpP impP, Backend::SP backend) {
    BddReturn result;
    ImpReturn impR = impUnion(a._impP,b._impP,impP);

    if (impR.first == UNSAT) {
        result.first = UNSAT;
    } else {
        ImpP impP = impR.second;
        const BddP bddAP = a._bddP;
        const BddP bddBP = b._bddP;
        if (bddAP == bddOne && bddBP == bddOne ){
            result.first = SAT;
            result.second = UpBdd(impP,bddOne);        
        } else {
            const Level level = maxLevel(bddAP,bddBP);
            adjustLevel(impP,level); // might change impP

            if (impliedLevel(impP,level) ) { // does the impP imply this level?
                return bddAPndCall(bddAP,bddBP,impP->getPos(level),impP,backend);
            } else {
                BddReturn highReturn = bddAPndCall(bddAP,bddBP,true,impP,backend);
                BddReturn lowReturn  = bddAPndCall(bddAP,bddBP,false,impP,backend);
                if (highReturn.first == SAT && lowReturn.first == SAT) {
                    result.first = SAT;
                    Bdd bdd(level, highReturn.second, lowReturn.second);
                    result.second = add(bdd);
                } else if (highReturn.first == UNSAT && lowReturn.first == UNSAT) {
                    result.first = UNSAT;
                } else if (highReturn.first == SAT && lowReturn.first == UNSAT) {
                    highReturn.second._impP->set(level,true); // todo!
                    result = highReturn;
                } else if (highReturn.first == UNSAT && lowReturn.first == SAT) {
                    lowReturn.second._impP->set(level,false); // todo!
                    result = lowReturn;
                }  
            }
        }
    }    
    return result;
}

BddReturn bddAndCall(Bdd& a, Bdd& b, ImpP impP, bool direction, Backend::SP backend) {
    if (a._level == b._level) {
        if (direction) {
            bddAnd(a._high, b._high, impP, backend);
        } else {
            bddAnd(a._low, b._low, impP, backend);
        }
    } else if (a._level > b._level) {
        if (direction) {
            bddAnd(a._high, b, impP, backend);
        } else {
            bddAnd(a._low, b, impP, backend);
        }
    } else {
        if (direction) {
            bddAnd(a, b._high, impP, backend);
        } else {
            bddAnd(a, b._low, impP, backend);
        }
    }
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
void testHashFunction();

int main () 
{
    //testSizes();
    //testImp();
    testBdd();
    //testHashFunction();
} 

bool testBdd1() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeBdd();
    Imp i1(1,0x0100,impOne); 
    Imp i2(1,0x0FF0,impOne); 
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    Bdd bdd1(2,bddOne,bddOne,iP1,iP1); 
    Bdd bdd2(3,bddOne,bddOne,iP1,iP2); 
    UpBdd upbdd1 = backend->add(bdd1);
    UpBdd upbdd2 = backend->add(bdd2);
    backend->debug();
    bool ok = upbdd1._bddP == bddOne && upbdd1._impP == iP1;
    return ok && before + 1 == backend->sizeBdd();
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
    UpBdd upBdd1 = backend->add(bdd1);
    UpBdd upBdd2 = backend->add(bdd2);
    Bdd bdd3(3,upBdd1,upBdd2); // 3 -> bdd1 ; bdd2
    backend->add(bdd3);
    backend->debug();
    return before + 3 == backend->sizeBdd();
}


void testBdd() {
    //std::cout << "testBdd 1: " << testBdd1() << std::endl;
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
    ImpReturn result = backend->impUnion(iP1,iP2);
    ok = result.first == SAT;
    ok = ok && result.second->_imp == i3._imp;
    ok = ok && before + 3 == backend->sizeImp();
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
    ImpReturn result = backend->impUnion(iP1,iP2);
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
    ImpReturn result = backend->impUnion(iP1,iP2);
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
    ImpReturn result = backend->impUnion(iP2,iP1);
    ok = result.first == SAT;
    ok = ok && before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}

bool testImp8() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0xF000,impOne); // 1
    Imp i3(2,0x000F,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ImpP iP4 = result.second;
    result = backend->impUnion(iP4,iP3);
    Imp i5(2,0xF00F,iP1);
    ImpP iP5 = backend->add(i5);
    ok = result.second == iP5;
    ok = ok && before + 5 == backend->sizeImp();
    //backend->debug();
    return ok;
}

bool testImp9() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0xF00F,impOne); // 1
    Imp i2(1,0xFF00,impOne); // 1
    Imp i3(1,0xF000,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP4 = backend->impIntersection(iP1,iP2);
    ok = iP4->_imp == i3._imp;
    ok = ok && before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}

bool testImp10() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0xF000,impOne); // 1
    Imp i3(2,0xFF00,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ImpP iP4 = result.second;
    ImpP iP5 = backend->impIntersection(iP3,iP4);
    ok = iP2 == iP5;
    ok = ok && before + 4 == backend->sizeImp();
    //backend->debug();
    return ok;
}

bool testImp11() {
    Backend::SP backend(new Backend(20,20));
    //size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0xF000,impOne); // 1
    Imp i3(2,0xFF00,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ImpP iP4 = result.second;
    ImpP iP5 = backend->impSubtraction(iP3,iP4);
    ImpP iP6 = backend->impSubtraction(iP4,iP3);
    backend->impUnion(iP5,iP6);
    ok = ok;// & iP5 && iP6 && before;
    backend->debug();
    return ok;
}

bool testImp12() {
    Backend::SP backend(new Backend(20,20));
    //size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); 
    Imp i2(2,0xF000,impOne); 
    Imp i3(2,0xFF00,impOne); 
    Imp i4(1,0x0000,impOne); 
    Imp i5(2,0x0000,impOne); 
    Imp i6(2,0x0000,impOne); 
    std::bitset<8> p(std::rand());
    i4.setPos(p);
    p = std::rand();
    i5.setNeg(p);
    p = std::rand();
    i6.setPos(p);
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpP iP4 = backend->add(i4);
    ImpP iP5 = backend->add(i5);
    ImpP iP6 = backend->add(i6);
    ImpReturn iP7 = backend->impUnion(iP5,iP6);
    backend->impIntersection(iP1,iP2);
    backend->impSubtraction(iP3,iP4);
    
    if (iP7.first == SAT ) {
        backend->impUnion(iP7.second,iP5);
    }
    backend->debug();
    return ok;
}

void testImp() {
    std::cout << "testImp 1: " << testImp1() << std::endl;
    std::cout << "testImp 2: " << testImp2() << std::endl;
    std::cout << "testImp 3: " << testImp3() << std::endl;
    std::cout << "testImp 4: " << testImp4() << std::endl;
    std::cout << "testImp 5: " << testImp5() << std::endl;
    std::cout << "testImp 6: " << testImp6() << std::endl;
    std::cout << "testImp 7: " << testImp7() << std::endl;
    std::cout << "testImp 8: " << testImp8() << std::endl;
    std::cout << "testImp 9: " << testImp9() << std::endl;
    std::cout << "testImp 10: " << testImp10() << std::endl;
    std::cout << "testImp 11: " << testImp11() << std::endl;
    std::cout << "testImp 12: " << testImp12() << std::endl;
}

void testSizes() {
    Level level = 123;
    std::bitset<16> bits = 0xF00F;
    Imp imp(level,bits,impOne);
    Bdd bdd(level,bddOne,bddOne,impOne,&imp);
    const UpBdd upbdd(&imp,&bdd);
    int a;
    long b=1234123;
    size_t c;
    std::cout << "size of imp " << sizeof(imp) << std::endl;
    std::cout << "size of bdd " << sizeof(bdd) << std::endl;
    std::cout << "size of upbdd " << sizeof(upbdd) << std::endl;
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

