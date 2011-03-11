#include <iostream>

#include "bddStore.h"
#include "impStore.h"
#include "backend.h"
#include "input.h"


using namespace std;

bool testBdd1() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeBdd();
    Imp i1(1,0x0100,impOne); 
    Imp i2(1,0x0FF0,impOne); 
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    Bdd bdd1(2,bddOne,bddOne,iP1,iP1); 
    Bdd bdd2(3,bddOne,bddOne,iP1,iP2); 
    UpBdd upbdd1 = backend->add(bdd1);
    backend->add(bdd2);
    //backend->debug();
    bool ok = upbdd1._bddP == bddOne && upbdd1._impP == iP1;
    return ok && before + 1 == backend->sizeBdd();
}

bool testBdd2() {
    Backend::SP backend(new Backend(20,20));
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0001,impOne); // -1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    Bdd bdd1(2,bddOne,bddOne,iP1,impOne); // 2 -> 1 
    Bdd bdd2(2,bddOne,bddOne,iP2,impOne); // 2 -> -1 
    UpBdd upBdd1 = backend->add(bdd1);
    UpBdd upBdd2 = backend->add(bdd2);
    BddReturn result = backend->bddAnd(upBdd1,upBdd2,impOne);
    //cout << "result is: " << result.first << " " << result.second.toString() << endl;
    //backend->debug();
    return true;
}

bool testBdd3() {
    Backend::SP backend(new Backend(20,20));
    Imp i1(1,0x0100,impOne); // 1
    //Imp i2(1,0x0001,impOne); // -1
    //Imp i3(1,0x0200,impOne); // 2
    ImpP iP1 = backend->add(i1);
    //ImpP iP2 = backend->add(i2);
    //ImpP iP3 = backend->add(i3);
    //Bdd bdd1(2,bddOne,bddOne,iP1,impOne); // 2 -> 1 
    Bdd bdd2(2,bddOne,bddOne,iP1,impOne); // 2 -> -1 
    Bdd bdd3(3,bddOne,bddOne,iP1,impOne); // 3 -> -1 
    //UpBdd upBdd1 = backend->add(bdd1);
    UpBdd upBdd2 = backend->add(bdd2);
    UpBdd upBdd3 = backend->add(bdd3);
    //UpBdd upBdd4(iP3,bddOne);
    BddReturn result = backend->bddAnd(upBdd2,upBdd3,impOne);
    //cout << "result is: " << result.first << " " << result.second.toString() << endl;
    //backend->debug();
    return result.first == SAT;
}

bool testBdd4() {
    Backend::SP backend(new Backend(20,20));
    Imp i1(1,0x0100,impOne); // 1
    //Imp i2(1,0x0001,impOne); // -1
    Imp i3(1,0x0200,impOne); // 2
    ImpP iP1 = backend->add(i1);
    //ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    //Bdd bdd1(2,bddOne,bddOne,iP1,impOne); // 2 -> 1 
    Bdd bdd2(2,bddOne,bddOne,iP1,impOne); // 2 -> 1 
    Bdd bdd3(3,bddOne,bddOne,iP1,impOne); // 3 -> 1 
    //UpBdd upBdd1 = backend->add(bdd1);
    UpBdd upBdd2 = backend->add(bdd2);
    UpBdd upBdd3 = backend->add(bdd3);
    //UpBdd upBdd4(iP3,bddOne);
    BddReturn result = backend->bddAnd(upBdd2,upBdd3,iP3);
    UpBdd re(result.second);
    //cout << "result is: " << result.first << " " << re.toString() << endl;
    //backend->debug();
    return re._bddP == bddOne && re._impP->getPosImp(1) && re._impP->getPosImp(2) && !re._impP->getPosImp(3);
}

bool testBdd5() {
    bool ok = true; 
    Backend::SP backend(new Backend(20,20));
    Clause c1;
    c1.push_back(1);
    c1.push_back(-2);
    Clause c2;
    c2.push_back(-1);
    c2.push_back(2);
    Clause c3;
    c3.push_back(-1);
    c3.push_back(-2);
    Clause c4;
    c4.push_back(1);
    c4.push_back(2);
    UpBdd up1 = backend->makeClause(c1);
    UpBdd up2 = backend->makeClause(c2);
    UpBdd up3 = backend->makeClause(c3);
    UpBdd up4 = backend->makeClause(c4);
    BddReturn result = backend->bddAnd(up1,up2);
    result = backend->bddAnd(result.second,up3);
    ok = ok && result.second._bddP == bddOne;
    result = backend->bddAnd(result.second,up4);
    ok = ok && result.first == UNSAT;
    //cout << "result is: " << result.first << endl << " " << result.second.toString() << endl;
    //backend->printClause(c1);
    //backend->printClause(c2);
    //backend->printClause(c3);
    //backend->printClause(c4);
    //backend->debug();
    return ok;
}


bool testBdd6() {
    bool ok = true; 
    Backend::SP backend(new Backend(20,20));
    Clause c1;
    c1.push_back(1);
    c1.push_back(-20);
    Clause c2;
    c2.push_back(-1);
    c2.push_back(20);
    Clause c3;
    c3.push_back(-1);
    c3.push_back(-20);
    Clause c4;
    c4.push_back(1);
    c4.push_back(20);
    UpBdd up1 = backend->makeClause(c1);
    UpBdd up2 = backend->makeClause(c2);
    UpBdd up3 = backend->makeClause(c3);
    UpBdd up4 = backend->makeClause(c4);
    BddReturn result = backend->bddAnd(up1,up2);
    //cout << "result of 1,2: " << result.first << " " << result.second.toString() << endl;
    result = backend->bddAnd(result.second,up3);
    //cout << "result of 1,2,3: " << result.first << " " << result.second.toString() << endl;
    ok = ok && result.second._bddP == bddOne;
    result = backend->bddAnd(result.second,up4);
    //cout << "result of 1,2,3,4: " << result.first << endl;
    ok = ok && result.first == UNSAT;
    //backend->printClause(c1);
    //backend->printClause(c2);
    //backend->printClause(c3);
    //backend->printClause(c4);
    //backend->debug();
    return ok;
}

bool testBdd7() {
    bool ok = true; 
    Backend::SP backend(new Backend(20,20));
    Clause c1;
    c1.push_back(1);
    c1.push_back(9);
    c1.push_back(-20);
    Clause c2;
    c2.push_back(-1);
    c2.push_back(9);
    c2.push_back(20);
    Clause c3;
    c3.push_back(-1);
    c3.push_back(9);
    c3.push_back(-20);
    Clause c4;
    c4.push_back(1);
    c4.push_back(9);
    c4.push_back(20);
    UpBdd up1 = backend->makeClause(c1);
    UpBdd up2 = backend->makeClause(c2);
    UpBdd up3 = backend->makeClause(c3);
    UpBdd up4 = backend->makeClause(c4);
    BddReturn result = backend->bddAnd(up1,up2);
    //cout << "result of 1,2: " << result.first << " " << result.second.toString() << endl;
    result = backend->bddAnd(result.second,up3);
    //cout << "result of 1,2,3: " << result.first << " " << result.second.toString() << endl;
    result = backend->bddAnd(result.second,up4);
    //cout << "result of 1,2,3,4: " << result.first << " " << result.second.toString() << endl;
    ok = ok && result.second._bddP == bddOne;
    ImpP impP = result.second._impP;
    ok = ok && impP->getPosImp(9);
    //backend->printClause(c1);
    //backend->printClause(c2);
    //backend->printClause(c3);
    //backend->printClause(c4);
    //backend->debug();
    return ok;
}

void testBdd() {
    cout << "testBdd 1: " << testBdd1() << endl;
    cout << "testBdd 2: " << testBdd2() << endl;
    cout << "testBdd 3: " << testBdd3() << endl;
    cout << "testBdd 4: " << testBdd4() << endl;
    cout << "testBdd 5: " << testBdd5() << endl;
    cout << "testBdd 6: " << testBdd6() << endl;
    cout << "testBdd 7: " << testBdd7() << endl;
}


