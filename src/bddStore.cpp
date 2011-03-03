#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/foreach.hpp>

#include "bdd.h"
#include "impStore.h"
#include "bddStore.h"

// BddStore

BddStore::BddStore(unsigned long size):
     store(size)
{
    BddP bOne = new Bdd(1,bddOne,bddOne,impOne,impOne);
    store[*bOne] = bddOne;
}

bool BddStore::lookup(const Bdd bdd) const {
    return store.find(bdd) != store.end();
}  

UpBdd BddStore::add(const Bdd bdd, ImpStore& impStore)  {
    UpBdd high = bdd._high;
    UpBdd low = bdd._low;
    if (high == low) {
        return high;
    }
    UpBdd result(impOne,bddOne);
    result._impP = impStore.impIntersection(high._impP,low._impP);
    high._impP = impStore.impSubtraction(high._impP,result._impP);
    low._impP = impStore.impSubtraction(low._impP,result._impP); 
    Bdd resBdd(bdd._level,high,low);
    BddStoreT::iterator i = store.find(resBdd);
    if (i != store.end() ) {
        result._bddP = i->second;
    } else {
        result._bddP = new Bdd(resBdd); 
        store[resBdd] = result._bddP;
    }
    return result;
}

void BddStore::debug() const {
    foreach(const BddStoreT::value_type& i , store) {
        if (i.second != bddOne) i.second->printBdd();
    };
}

size_t BddStore::size() {
    return store.size();
}
