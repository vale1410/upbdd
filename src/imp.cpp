#include <iostream>
#include <sstream>
#include <boost/functional/hash.hpp>

#include "imp.h"

Imp::Imp(Level level, std::bitset<2*8> imp, ImpP next):
    _level(level),
    _imp(imp),
    _nextP(next)
{}

std::string Imp::toString() const {
    std::ostringstream oss;
    oss << this << " = " << "(" << _level <<  "," << pos() <<  "," << neg() << "," << _nextP <<  ")";
    return oss.str();
}

void Imp::setPos(std::bitset<8> bits) {
    for(size_t i= 0 ; i<_imp.size()/2;i++) {
        setPos(i,bits[7-i]);
    }
}

void Imp::setNeg(std::bitset<8> bits) {
    for(size_t i= 0 ; i<_imp.size()/2;i++) {
        setNeg(i,bits[7-i]);
    }
}

std::bitset<8> Imp::getPos() const {
    std::bitset<8> result;
    for(size_t i= 0 ; i<_imp.size()/2;i++) {
        result[7-i] = getPos(i);
    }
    return result;
}

std::bitset<8> Imp::getNeg() const {
    std::bitset<8> result;
    for(size_t i= 0 ; i<_imp.size()/2;i++) {
        result[7-i] = getNeg(i);
    }
    return result;
}

bool Imp::getPos(size_t i) const {
    return _imp.test(_imp.size()-1-i);
}

bool Imp::getNeg(size_t i) const {
    return _imp.test(_imp.size()/2-1-i);
}

void Imp::setPos(size_t i,bool b) {
    _imp[_imp.size()-1-i] = b;
}

void Imp::setNeg(size_t i, bool b) {
    _imp[_imp.size()/2-1-i] = b;
}

std::string Imp::pos() const {
    std::ostringstream oss;
    oss << getPos();
    return oss.str();
}

std::string Imp::neg() const {
    std::ostringstream oss;
    oss << getNeg();
    return oss.str();
}

void Imp::printImp()
{
    ImpHashFunction hasher;
    std::cout << "imp: " << toString() << "\t hash: " << hasher(*this) << std::endl;
}


bool ImpEqual::operator() (const Imp& a, const Imp& b) const {
    return (a._level ==  b._level &&
            a._imp   ==  b._imp  &&
            a._nextP  ==  b._nextP);
}
        
