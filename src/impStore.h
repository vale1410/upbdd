#ifndef impStore_h
#define impStore_h

#include <boost/unordered_map.hpp>

#include "imp.h"
#include "common.h"

typedef boost::unordered_map< Imp, ImpP, ImpHashFunction, ImpEqual > ImpStoreT;

class ImpStore {
    public: 

        ImpStore(unsigned long size);

        bool lookup(const Imp imp) const;

        ImpP add(const Imp imp);

        void debug();

        size_t size();

        ImpP impSubtraction(ImpP a, ImpP b);

        ImpP impIntersection(ImpP a, ImpP b);
        
        ImpReturn impUnion(ImpP a, ImpP b);
        ImpReturn impUnion(ImpP a, ImpP b, ImpP c);
        
        //TODO: not implemented yet exception
        bool impliedLevel(ImpP imp, Level level);

        //TODO: not implemented yet exception
        ImpP adjustLevel(ImpP, Level);

        //TODO: not implemented yet exception
        ImpP makeNewImpWithLevel(ImpP, Level, bool direction);

    private:
        ImpStoreT store;
};

#endif
