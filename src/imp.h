#ifndef imp_h
#define imp_h

#include <boost/functional/hash.hpp>
#include <cstdlib>
#include <bitset>

#include "common.h"

/*
 * level 0 does not exist! everything starts with level 1. 
 * A block is sofar 8 levels, and each imp has exactly 1 block. 
 * so:
 * level 1, is level 1 in block 1. 
 * level 8, is level 8 in block 1. 
 * level 9 is level 1 in block 2. 
 * level 59 is level 3 in block 8. 
 * level n is level n%8 in block (n-1)/8 + 1.// if n%8==0 then give 8
 */

class Imp;
typedef Imp* ImpP;

typedef std::pair<Feedback,ImpP> ImpReturn;

class Imp {

    public:

        Block _block;
        std::bitset<2*8> _imp;
        ImpP _nextP;

        Imp(Block block, std::bitset<2*8> imp, ImpP next);
        
        std::string toString() const;

        void setPos(std::bitset<8> bits);
        void setNeg(std::bitset<8> bits);
        
        std::bitset<8> getPos() const;
        std::bitset<8> getNeg() const;

        void setNegImp(Level);
        void setPosImp(Level);

        bool getNegImp(Level);
        bool getPosImp(Level);

        std::string pos() const;
        std::string neg() const;

        void printImp();
    
        bool getPos(size_t pos) const;
        bool getNeg(size_t pos) const;
    
    private:

        /* 
         * the following four functions are technical, i.e
         * i referes to the technical position (starting from 0). 
         * the first bit in the positive vector is at position 0
         */

        void setPos(size_t pos, bool b);
        void setNeg(size_t pos, bool b);

};

class ImpEqual : std::equal_to< Imp > {
    public:
        bool operator() (const Imp& a, const Imp& b) const;
};

class ImpHashFunction : std::unary_function< Imp , size_t > {
    public:

    inline size_t operator() (const Imp& imp) const {
        boost::hash<ImpP> hasher;
        std::size_t hash = imp._imp.to_ulong() + hasher(imp._nextP) + imp._block;
        boost::hash_combine(hash, imp._imp.to_ulong());
        boost::hash_combine(hash, imp._nextP);
        boost::hash_combine(hash, imp._block);
        return hash;
    }

}; 

#endif
