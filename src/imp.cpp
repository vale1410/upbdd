#include <iostream>
#include <sstream>
#include <boost/functional/hash.hpp>

#include "imp.h"

Imp::Imp():
    _block(0),
    _imp(0x0000),
    _nextP(impOne)
{}

Imp::Imp(Block block, std::bitset<2*8> imp, ImpP next):
    _block(block),
    _imp(imp),
    _nextP(next)
{}

Imp::Imp(Block block, std::bitset<8> posImp, std::bitset<8> negImp, ImpP next):
    _block(block),
    _nextP(next)
{
    setPos(posImp);
    setNeg(negImp);
}


std::string Imp::toString() const {
    std::ostringstream oss;
    oss << this << " = " << "(" << _block <<  "," << posToString() <<  "," << negToString() << "," << _nextP <<  ")";
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

bool Imp::getNegImp(Level level) const {
    return getNeg(8-level2pos(level));
}

bool Imp::getPosImp(Level level) const {
    return getPos(8-level2pos(level));
}

void Imp::setNegImp(Level level) {
    setNeg(8-level2pos(level),true);
}

void Imp::setPosImp(Level level) {
    setPos(8-level2pos(level),true);
}

std::string Imp::posToString() const {
    std::ostringstream oss;
    oss << getPos();
    return oss.str();
}

std::string Imp::negToString() const {
    std::ostringstream oss;
    oss << getNeg();
    return oss.str();
}

void Imp::printImp()
{
    ImpHashFunction hasher;
    std::cout << "imp: " << toString() << "\t hash: " << hasher(*this) << std::endl;
}

Block Imp::level2block(Level level) const {
    size_t block = level/8 + 1;
    return block;
}

size_t Imp::level2pos(Level level) const {
    size_t pos = level % 8;
    if (pos == 0) pos = 8;
    return pos;
}

Level Imp::pos2level(size_t pos) const {
    return 8 * (_block-1) + pos;
}

bool ImpEqual::operator() (const Imp& a, const Imp& b) const {
    return (a._block ==  b._block &&
            a._imp   ==  b._imp  &&
            a._nextP  ==  b._nextP);
}
        
