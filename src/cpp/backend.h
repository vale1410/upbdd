#ifndef backend_h
#define backend_h

#include <boost/shared_ptr.hpp>

#include "bdd.h"
#include "imp.h"
#include "impStore.h"
#include "bddStore.h"
#include "andStore.h"

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
      void printSize();

      Level maxLevel(BddP a,BddP b);

      BddReturn bddAnd(std::vector< UpBdd >);
      BddReturn bddAnd(UpBdd, UpBdd);
      BddReturn bddAnd(UpBdd, UpBdd, ImpP);
      BddReturn bddAndCall(const BddP, const BddP, const ImpP, const bool direction);

      /* 
       * precondition: Clause is sorted, and no double entries
       * Clause has at least one element!;
       */

      UpBdd makeClause(Clause);
      std::vector<UpBdd> makeClauses(RawProblem problem);
      ImpP makeImplication(RawImplication);
      void printClause(Clause);
        
   private:
       BddStore bddStore;
       ImpStore impStore;
       AndStore andStore;  // cache for andBdd
};

#endif

