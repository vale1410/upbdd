#include <iostream>
#include <sstream>
#include <boost/functional/hash.hpp>

#include "imp.h"

Imp::Imp(Block block, std::bitset<2*8> imp, ImpP next):
    _block(block),
    _imp(imp),
    _nextP(next)
{}

std::string Imp::toString() const {
    std::ostringstream oss;
    oss << this << " = " << "(" << _block <<  "," << pos() <<  "," << neg() << "," << _nextP <<  ")";
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

bool Imp::getPos(size_t pos) const {
    return _imp.test(_imp.size()-1-pos);
}

bool Imp::getNeg(size_t pos) const {
    return _imp.test(_imp.size()/2-1-pos);
}

void Imp::setPos(size_t pos,bool b) {
    _imp[_imp.size()-1-pos] = b;
}

void Imp::setNeg(size_t pos, bool b) {
    _imp[_imp.size()/2-1-pos] = b;
}

bool Imp::getNegImp(Level level) {
    size_t pos = level % 8;
    if (pos == 0) pos = 8;
    return getNeg(8-level);
}

bool Imp::getPosImp(Level level) {
    size_t pos = level % 8; 
    if (pos == 0) pos = 8;
    return getPos(8-level);
}

void Imp::setNegImp(Level level) {
    size_t pos = level % 8;
    if (pos == 0) pos = 8;
    setNeg(8-level,true);
}

void Imp::setPosImp(Level level) {
    size_t pos = level % 8;
    if (pos == 0) pos = 8;
    setPos(8-level,true);
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
    return (a._block ==  b._block &&
            a._imp   ==  b._imp  &&
            a._nextP  ==  b._nextP);
}
        
