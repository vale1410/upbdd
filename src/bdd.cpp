#include <iostream>
#include <sstream>

#include "bdd.h"


UpBdd::UpBdd(ImpP impP,BddP bddP):
        _bddP(bddP),
        _impP(impP)
    {} 

bool UpBdd::operator==(const UpBdd &other) const {
    return _bddP == other._bddP && _impP == other._impP;
}

std::string UpBdd::toString() const {
    std::ostringstream oss;
    oss << " <" << _impP <<  "," << _bddP <<  ">";
    return oss.str();
}


Bdd::Bdd(Level level, UpBdd high, UpBdd low) :
    _level(level),
    _high(high),
    _low(low)
    {}
            
Bdd::Bdd(Level level, BddP highP, BddP lowP, ImpP hImpP, ImpP lImpP) :
    _level(level),
    _high(hImpP,highP),
    _low(lImpP,lowP)
    {}


std::string Bdd::toString() const {
    std::ostringstream oss;
    oss << this << " = " << "(" << _level <<  ",<" << _high._impP <<  "," << _high._bddP <<  ">,<" << _low._impP <<  "," << _low._bddP <<  ">)";
    return oss.str();
}

void Bdd::printBdd()
{
    BddHashFunction hasher;
    std::cout << "bdd: " << toString() << "\t hash: " << hasher(*this) << std::endl;
}


 bool UpBddEqual::operator() (const UpBdd& a, const UpBdd& b) const {
     return (a._bddP ==  b._bddP &&
             a._impP  ==  b._impP);
 }

bool BddEqual::operator() (const Bdd& a, const Bdd& b) const {
     return (a._level ==  b._level &&
             a._high  ==  b._high  &&
             a._low   ==  b._low);
}
