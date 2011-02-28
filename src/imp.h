#ifndef imp_h
#define imp_h

#include <boost/functional/hash.hpp>
#include <cstdlib>
#include <bitset>

#include "common.h"

class Imp;
typedef Imp* ImpP;

typedef std::pair<Feedback,ImpP> ImpReturn;

class Imp {

    public:

    Level _level;
    std::bitset<2*8> _imp;
    ImpP _nextP;

    Imp(Level level, std::bitset<2*8> imp, ImpP next);
    
    std::string toString() const;

    void setPos(std::bitset<8> bits);
    void setNeg(std::bitset<8> bits);
    
    std::bitset<8> getPos() const;
    std::bitset<8> getNeg() const;
    
    bool getPos(size_t i) const;
    bool getNeg(size_t i) const;

    void setPos(size_t i,bool b);
    void setNeg(size_t i, bool b);

    std::string pos() const;
    std::string neg() const;

    void printImp();

};

class ImpEqual : std::equal_to< Imp > {
    public:
        bool operator() (const Imp& a, const Imp& b) const;
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

#endif
