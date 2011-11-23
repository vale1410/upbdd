#ifndef bddStore_h
#define bddStore_h

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

#endif
