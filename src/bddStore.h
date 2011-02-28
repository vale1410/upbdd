#ifndef bddStore_h
#define bddStore_h

#include <boost/shared_ptr.hpp>

#include "bdd.h"
#include "imp.h"
#include "impStore.h"


typedef boost::unordered_map< Bdd, BddP, BddHashFunction, BddEqual > BddStoreT;


class BddStore {
    public: 

        BddStore(unsigned long size);

        bool lookup(const Bdd bdd) const;
        
        UpBdd add(const Bdd bdd, ImpStore& impStore);
       
        void debug() const;
        
        size_t size();
    
    private:
        BddStoreT store;
};


class Backend {

    public:
    
        typedef boost::shared_ptr<Backend> SP;
        typedef const boost::shared_ptr<Backend> ConstSP;

        Backend(unsigned int bddSize, unsigned int impSize);

        UpBdd add(const Bdd bdd);

        ImpP add(const Imp imp);

        ImpReturn impUnion(ImpP a, ImpP b);
        ImpReturn impUnion(ImpP a, ImpP b, ImpP c);
        ImpP impIntersection(ImpP a, ImpP b);

        ImpP impSubtraction(ImpP a, ImpP b);

        void debug();

        size_t sizeImp();
        size_t sizeBdd();

        //TODO: not implemented yet exception
        bool impliedLevel(ImpP imp, Level level);

        //TODO: not implemented yet exception
        ImpP adjustLevel(ImpP imp, Level level);

        //TODO: not implemented yet exception
        Level maxLevel(BddP a,BddP b);

        BddReturn bddAnd(UpBdd, UpBdd, ImpP);
        BddReturn bddAndCall(const BddP, const BddP, const ImpP, const bool direction);
        
   private:
       BddStore bddStore;
       ImpStore impStore;
       //BddAndStore bddAndStore;  // cache for andBdd
};

#endif
