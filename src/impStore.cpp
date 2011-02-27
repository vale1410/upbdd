#ifndef stores_cpp
#define stores_cpp

#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>

#include "common.h"
#include "imp.cpp"

typedef boost::unordered_map< Imp, ImpP, ImpHashFunction, ImpEqual > ImpStoreT;


class ImpStore {
    public: 

        ImpStore(unsigned long size):
             store(size)
        {
            ImpP iOne = new Imp(1,0x0000,impOne);
            store[*iOne] = impOne;
        }

        bool lookup(const Imp imp) const {
            return store.find(imp) != store.end();
        }  

        ImpP add(const Imp imp) {
            //std::cout << "imp add: " << imp.toString() << std::endl;
            if (!imp._imp.any()) {
                return imp._nextP;
            }
            
            ImpStoreT::iterator i = store.find(imp);
            ImpP result;
            if (i != store.end() ) {
                result = i->second;
            } else {
                result = new Imp(imp);
                store[imp]=result;
            }            
            return result;
        }

        void debug() {
            BOOST_FOREACH(const ImpStoreT::value_type& i , store) {
                if (i.second != impOne) printImp(i.second);
            };
        }

        size_t size() {
            return store.size();
        }

        // a - b = a XOR impIntersection(a,b)
        ImpP impSubtraction(ImpP a, ImpP b) {
            if (a == impOne) {
                return impOne;
            } else if (b == impOne ) {
                return a;
            } else if (a->_level == b->_level) {
                Imp imp = Imp(a->_level,0x0000,impOne);
                imp.setPos(a->getPos() ^ (a->getPos() & b->getPos()));
                imp.setNeg(a->getNeg() ^ (a->getNeg() & b->getNeg()));
                imp._nextP = impSubtraction(a->_nextP,b->_nextP);
                return add(imp);
            } else if (a->_level < b->_level) {
                return impSubtraction(a,b->_nextP);
            } else {
                return impSubtraction(a->_nextP,b);
            }
        }
        
        ImpP impIntersection(ImpP a, ImpP b) {
            if (a == impOne || b == impOne ) {
                return impOne;
            } else if (a->_level == b->_level) {
                Imp imp = Imp(1,0x0000,impOne);
                imp._level = a->_level;
                imp.setPos(a->getPos() & b->getPos());
                imp.setNeg(a->getNeg() & b->getNeg());
                imp._nextP = impIntersection(a->_nextP,b->_nextP);
                return add(imp);
            } else if (a->_level < b->_level) {
                return impIntersection(a,b->_nextP);
            } else {
                return impIntersection(a->_nextP,b);
            }
        }
        

        ImpReturn impUnion(ImpP a, ImpP b) {
            ImpReturn result;
            if (a == impOne) {
                result.first = SAT;
                result.second = b;
                return result;
            }
            else if (b == impOne) {
                result.first = SAT;
                result.second = a;
                return result;
            }

            Imp imp(0,0x0000,NULL);
            ImpP nextA;
            ImpP nextB; 
            
            if (a->_level == b->_level) {
                imp._level = a->_level;
                imp.setPos(a->getPos() | b->getPos());
                imp.setNeg(a->getNeg() | b->getNeg());
                if(imp.getPos().to_ulong() & imp.getNeg().to_ulong()) {
                    result.first = UNSAT;
                    return result;
                } else {
                    nextA = a->_nextP;
                    nextB = b->_nextP;
                }
            } else if (a->_level < b->_level) {
                nextA = a;
                nextB = b->_nextP;
                imp._level = b->_level;
                imp._imp = b->_imp;
            } else {
                nextA = a->_nextP;
                nextB = b;
                imp._level = a->_level;
                imp._imp = a->_imp; 
            }
            
            ImpReturn recursion = impUnion(nextA,nextB);
        
            if (recursion.first == UNSAT) {
                result = recursion;
            } else {
                result.first = SAT;
                imp._nextP = recursion.second;
                result.second = add(imp);
            }
            return result;
        }

        ImpReturn impUnion(ImpP a, ImpP b, ImpP c) {
            ImpReturn result = impUnion(a,b);
            if (result.first == SAT) {
                result = impUnion(result.second,c);
            }
            return result;
        }

    private:
        ImpStoreT store;
};

#endif
