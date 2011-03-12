#ifndef andStore_h
#define andStore_h

#include "bdd.h"
#include "bddStore.h"

class Entry {
    public:
        Entry(BddP a, BddP b, ImpP impP);

    const BddP _a ,_b;
    const ImpP _impP;
}

class EntryHashFunction : std::unary_function< Entry , size_t > {
    public:
        inline size_t operator() (const Entry& entry) const {
            std::size_t hash = 0;
            boost::hash_combine(hash, entry._a);
            boost::hash_combine(hash, entry._b);
            boost::hash_combine(hash, entry._impP);
            return hash;
        }
};

class EntryEqual : std::equal_to< Entry > {
    public:
        bool operator() (const Entry& a, const Entry& b) const {
            return a._a == b._a && a._b == b._b && a._impP == b._impP;
        }

};


typedef boost::unordered_map< Entry, BddReturn, EntryHashFunction, EntryEqual > AndStoreT;


class AndStore {
    public: 

        AndStore(unsigned long size);

        bool lookup(const BddP a, const BddP b, const ImpP impP, BddReturn& result) const;
        
        void add(const BddP a, const BddP b, const ImpP impP, BddReturn result);
       
        void clear();

        void debug() const;
        
        size_t size() const;
    
    private:
        AndStoreT store;
};

#endif

