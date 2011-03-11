#ifndef imp_h
#define imp_h

#include <boost/functional/hash.hpp>
#include <cstdlib>
#include <bitset>

#include "common.h"

/*
 * level 0 does not exist! everything starts with level 1. 
 * A block is up to now 8 levels, and each imp has exactly 1 block. 
 * so:
 * level 1 is position 1 in block 1. 
 * level 8 is position 8 in block 1. 
 * level 9 is position 1 in block 2. 
 * level 59 is position 3 in block 8. 
 * level n is position n%8 in block (n-1)/8 + 1.// if n%8==0 then 8
 */

class Imp;
typedef Imp* ImpP;

typedef std::pair<Feedback,ImpP> ImpReturn;

class Imp {

    public:

        Block _block;
        std::bitset<2*8> _imp;
        ImpP _nextP;
        
        Imp();
        Imp(Block block, std::bitset<2*8> imp, ImpP next);
        Imp(Block block, std::bitset<8> posImp, std::bitset<8> negImp, ImpP next);

        std::bitset<8> getPos() const;
        std::bitset<8> getNeg() const;

        void setNegImp(Level);
        void setPosImp(Level);

        bool getNegImp(Level) const;
        bool getPosImp(Level) const;

        ImpP adjustLevel(Level) const;

        std::string toString() const;

        void printImp();
        
        Block level2block(Level level) const;
    
    private:

        /* 
         * the following four functions are technical, i.e
         * i referes to the technical position (starting from 0). 
         * the first bit in the positive vector is at position 0
         */

        void setPos(size_t pos, bool b);
        void setNeg(size_t pos, bool b);
    
        bool getPos(size_t pos) const;
        bool getNeg(size_t pos) const;

        void setPos(std::bitset<8> bits);
        void setNeg(std::bitset<8> bits);
        
        size_t level2pos(Level level) const;
        Level pos2level(size_t pos) const;
        
        std::string posToString() const;
        std::string negToString() const;

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
