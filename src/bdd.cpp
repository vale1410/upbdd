#include <iostream>
#include <string>
#include <sstream>
//#include <stdlib.h>
#include <bitset>
//#include <math.h>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>

class Imp;
class Bdd;
class ImpHashFunction;
class BddHashFunction;

typedef unsigned int Level;
typedef const Imp* ImpP;
typedef const Bdd* BddP;


class Imp {
    public:


    Imp(const Level level, std::bitset<2*8> imp, ImpP next):
        _level(level),
        _imp(imp),
        _next(next)
    {}

    const Level _level;
    const std::bitset<2*8> _imp;
    const ImpP _next;
    
    std::string toString() const {
        std::ostringstream oss;
        oss << this << " = " << "(" << _level <<  "," << _imp <<  "," << _next <<  ")";
        return oss.str();
    }


};


class Bdd {
    public:
        
    Bdd(const Level level, BddP high, BddP low, ImpP hImp, ImpP lImp) :
        _level(level),
        _high(high),
        _low(low),
        _hImp(hImp),
        _lImp(lImp) 
        {}

     const Level _level;
     const BddP _high;
     const BddP _low;
     const ImpP _hImp;
     const ImpP _lImp;
     
     std::string toString() const {
         std::ostringstream oss;
         oss << this << " = " << "(" << _level <<  "," << _high <<  "," << _low <<  "," << _hImp <<  "," << _lImp <<  ")";
         return oss.str();
     }
};
    


const BddP bddOne = NULL;
const ImpP impOne = NULL;


class Storage;

class ImpHashFunction : std::unary_function< Imp , size_t > {
    public:
        inline size_t operator() (const Imp& imp) const {
            std::size_t hash = 0;
            boost::hash_combine(hash, imp._level);
            boost::hash_combine(hash, imp._imp.to_ulong());
            boost::hash_combine(hash, imp._next);
            return hash;
        }
};

class BddHashFunction : std::unary_function< Bdd , size_t > {
    public:
        inline size_t operator() (const Bdd& bdd) const {
            std::size_t hash = 0;
            boost::hash_combine(hash, bdd._level);
            boost::hash_combine(hash, bdd._high);
            boost::hash_combine(hash, bdd._low);
            boost::hash_combine(hash, bdd._hImp);
            boost::hash_combine(hash, bdd._lImp);
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

inline void printImp(ImpP imp)
{
    ImpHashFunction hasher;
    std::cout << "imp: " << imp->toString() << "\t hash: " << hasher(*imp) << std::endl;
}

inline void printBdd(BddP bdd)
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
        {}

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
    
    private:
        BddStoreT store;
};

class ImpStore {
    public: 

        ImpStore(unsigned long size):
             store(size)
        {}

        bool lookup(const Imp bdd) const {
            return store.find(bdd) != store.end();
        }  

        ImpP add(const Imp bdd)  {
            ImpStoreT::iterator i = store.find(bdd);
            ImpP result;
            if (i != store.end() ) {
                result = i->second;
            } else {
                result = new Imp(bdd);
                store[bdd]=result;
            }            
            return result;
        }
    
    private:
        ImpStoreT store;
};

class Backend {
    public:
        Backend(unsigned int bddSize, unsigned int impSize):
            bddStore(bddSize),
            impStore(impSize)
        {}

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
    private:
        BddStore bddStore;
        ImpStore impStore;
};


int main () 
{
    const Level level = 0;
    const std::bitset<16> bits = 0xFF;
    const Imp imp(level,bits,impOne);
    const Bdd bdd1(level,bddOne,bddOne,impOne,&imp);
    const Bdd bdd2(level+1,&bdd1,bddOne,impOne,&imp);
    const Bdd bdd(level+2,&bdd1,&bdd2,impOne,&imp);
    std::cout << "first tests for bdds" << std::endl;
    printImp(&imp);
    printBdd(&bdd1);
    printBdd(&bdd2);
    printBdd(&bdd);
    //int a;
    //long b;
    //size_t c;
    std::cout << "size of imp " << sizeof(imp) << std::endl;
    std::cout << "size of bdd " << sizeof(bdd) << std::endl;
    //std::cout << "size of imp.imp " << sizeof(imp._imp) << std::endl;
    //std::cout << "size of int " << sizeof(a) << std::endl;
    //std::cout << "size of long " << sizeof(b) << std::endl;
    //std::cout << "size of size_t " << sizeof(c) << std::endl;
    std::cout << "output of bddOne " << bddOne << std::endl;
    std::cout << "output of impOne " << impOne << std::endl;
} 

