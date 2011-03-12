#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/foreach.hpp>

#include "andStore.h"


Entry::Entry(BddP a, BddP b, ImpP impP):
    _a(a),
    _b(b),
    _impP(impP)
{}

AndStore::AndStore(unsigned long size):
    store(size)
{}


 bool AndStore::lookup(const BddP a, const BddP b, const ImpP impP, BddReturn& result) const {
    Entry entry(a,b,impP);
    if (store.find(entry) != store.end()) {
        return store.find(entry);
    } else {
        return false;
    }
}
        
void AndStore::add(const BddP a, const BddP b, const ImpP impP, BddReturn result) {
    Entry entry(a,b,impP);
    store[entry] = result;
}
      
void AndStore::clear() {
    store.clear();
}

void debug() const {
    foreach(const AndStoreT::value_type& i , store) {
        i.first->toString()//TODO
    };
}
        
        size_t size() const;
    
    private:
        AndStoreT store;
};



