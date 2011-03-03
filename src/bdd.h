#ifndef bdd_h
#define bdd_h

#include <string>
#include <bitset>
#include <boost/functional/hash.hpp>

#include "common.h"
#include "imp.h"

class Bdd;
typedef Bdd* BddP;

class UpBdd;
typedef std::pair<Feedback,UpBdd> BddReturn;

class UpBdd {
    public:

        UpBdd(ImpP impP,BddP bddP);
        UpBdd();

        bool operator==(const UpBdd &other) const;
        
        std::string toString() const;

    BddP _bddP;
    ImpP _impP;

};

class Bdd {
    public:
        
        Bdd();
        Bdd(Level level, UpBdd high, UpBdd low);
        Bdd(Level level, BddP highP, BddP lowP, ImpP hImpP, ImpP lImpP);

        void printBdd();
        std::string toString() const;

        Level _level;
        UpBdd _high; 
        UpBdd _low;
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
        bool operator() (const UpBdd& a, const UpBdd& b) const;
};

class BddEqual : std::equal_to< Bdd > {
    public:
        bool operator() (const Bdd& a, const Bdd& b) const ;
};

#endif
