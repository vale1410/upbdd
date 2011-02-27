#ifndef imp_cpp
#define imp_cpp

#include <iostream>
#include <bitset>
#include <sstream>
#include <boost/functional/hash.hpp>

#include "common.h"


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

void printImp()
{
    ImpHashFunction hasher;
    std::cout << "imp: " << toString() << "\t hash: " << hasher(*this) << std::endl;
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
