#include <iostream>

#include "bddStore.h"
#include "impStore.h"
#include "backend.h"
#include "input.h"


using namespace std;

void testSizes();
void testImp();
void testBdd();
void testInput();
void testHashFunction();

int main () 
{
    //testSizes();
    testInput();
    //testImp();
    //testBdd();
    //testHashFunction();
    return 0;
} 

bool testInput1() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem problem; 
    parseProblem("data/testInput1.cnf", problem);
    cout << "c what did I read: " << endl;
    UpBdd up;
    vector<bool> has_results;
    vector<BddReturn> results;
    
    has_results.push_back(false);
    results.push_back(BddReturn());

    has_results.push_back(false);
    results.push_back(BddReturn());
    
    has_results.push_back(true);
    results.push_back(BddReturn(SAT,UpBdd(backend->makeImplication(problem[0]),bddOne)));
   
    has_results.push_back(true);
    results.push_back(BddReturn(UNSAT,UpBdd()));

    int i = 0;
    foreach(Clause clause, problem) {
        foreach(Literal variable, clause) {
                cout << variable << " ";
        };
        cout << endl;
        UpBdd tmp = backend->makeClause(clause);
        BddReturn result = backend->bddAnd(up,tmp);
        cout << "bddAnd(" << up.toString() << ", " << tmp.toString();
        if (result.first == SAT) {
            cout << ") = (SAT, " << result.second.toString() << ")\n";
            up = result.second;
        } else {
            cout << ") =  UNSAT" << endl;
        }
        if (has_results[i]) {
            ok = ok && results[i] == result;
        }
        i++;
   };
   backend->debug(); 
   return ok;
}

bool testInput2() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem problem; 
    parseProblem("data/testInput2.cnf", problem);
    cout << "c what did I read: " << endl;
    UpBdd up;
    vector<bool> has_results;
    vector<BddReturn> results;
    
    has_results.push_back(false);
    results.push_back(BddReturn());

    has_results.push_back(false);
    results.push_back(BddReturn());
    
    has_results.push_back(true);
    results.push_back(BddReturn(SAT,UpBdd(backend->makeImplication(problem[0]),bddOne)));
   
    has_results.push_back(true);
    results.push_back(BddReturn(UNSAT,UpBdd()));

    int i = 0;
    foreach(Clause clause, problem) {
        foreach(Literal variable, clause) {
                cout << variable << " ";
        };
        cout << endl;
        UpBdd tmp = backend->makeClause(clause);
        BddReturn result = backend->bddAnd(up,tmp);
        cout << "bddAnd(" << up.toString() << ", " << tmp.toString();
        if (result.first == SAT) {
            cout << ") = (SAT, " << result.second.toString() << ")\n";
            up = result.second;
        } else {
            cout << ") =  UNSAT" << endl;
        }
        if (has_results[i]) {
            ok = ok && results[i] == result;
        }
        i++;
   };
   backend->debug(); 
   return ok;
}


bool testInput3() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem problem; 
    parseProblem("data/testInput3.cnf", problem);
    cout << "c what did I read: " << endl;
    UpBdd up;
    vector<bool> has_results;
    vector<BddReturn> results;
    
    foreach(Clause clause, problem) {
        foreach(Literal variable, clause) {
                cout << variable << " ";
        };
        cout << endl;
        UpBdd tmp = backend->makeClause(clause);
        BddReturn result = backend->bddAnd(up,tmp);
        cout << "bddAnd(" << up.toString() << ", " << tmp.toString();
        if (result.first == SAT) {
            cout << ") = (SAT, " << result.second.toString() << ")\n";
            up = result.second;
        } else {
            cout << ") =  UNSAT" << endl;
        }
   };
   backend->debug(); 
   return ok;
}

void testInput(){
    cout << "testInput 1: " << testInput1() << endl;
    cout << "testInput 2: " << testInput2() << endl;
    cout << "testInput 3: " << testInput2() << endl;
}

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
    backend->debug();
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


bool testImp1() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0100,impOne); // 1
    backend->add(i1);
    backend->add(i2);
    return before + 1 == backend->sizeImp();
}

bool testImp2() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0110,impOne); // 1
    backend->add(i1);
    backend->add(i2);
    return before + 2 == backend->sizeImp();
}


bool testImp3() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0x0100,impOne); // 1
    backend->add(i1);
    backend->add(i2);
    return before + 2 == backend->sizeImp();
}


bool testImp4() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0010,impOne); // -5
    Imp i3(1,0x0110,impOne); // 1 /\ -5
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ok = result.first == SAT;
    ok = ok && result.second->_imp == i3._imp;
    ok = ok && before + 3 == backend->sizeImp();
    return ok;
}


bool testImp5() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0001,impOne); // -1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ok = result.first == UNSAT;
    ok = ok && before + 2 == backend->sizeImp();
    return ok;
}


bool testImp6() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0x0001,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ok = result.first == SAT;
    ok = ok && before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}


bool testImp7() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0x0001,impOne); // -1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpReturn result = backend->impUnion(iP2,iP1);
    ok = result.first == SAT;
    ok = ok && before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}

bool testImp8() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0xF000,impOne); // 5 and 6 and 7 and 8
    Imp i3(2,0x000F,impOne); // -1 and -2 and -3 and -4
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ImpP iP4 = result.second;
    result = backend->impUnion(iP4,iP3);
    Imp i5(2,0xF00F,iP1);
    ImpP iP5 = backend->add(i5);
    ok = result.second == iP5;
    ok = ok && before + 5 == backend->sizeImp();
    //backend->debug();
    return ok;
}

bool testImp9() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0xF00F,impOne); // 1
    Imp i2(1,0xFF00,impOne); // 1
    Imp i3(1,0xF000,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP4 = backend->impIntersection(iP1,iP2);
    ok = iP4->_imp == i3._imp;
    ok = ok && before + 3 == backend->sizeImp();
    //backend->debug();
    return ok;
}

bool testImp10() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0xF000,impOne); // 1
    Imp i3(2,0xFF00,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ImpP iP4 = result.second;
    ImpP iP5 = backend->impIntersection(iP3,iP4);
    ok = iP2 == iP5;
    ok = ok && before + 4 == backend->sizeImp();
    //backend->debug();
    return ok;
}

bool testImp11() {
    Backend::SP backend(new Backend(20,20));
    //size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(2,0xF000,impOne); // 1
    Imp i3(2,0xFF00,impOne); // 1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpReturn result = backend->impUnion(iP1,iP2);
    ImpP iP4 = result.second;
    ImpP iP5 = backend->impSubtraction(iP3,iP4);
    ImpP iP6 = backend->impSubtraction(iP4,iP3);
    backend->impUnion(iP5,iP6);
    ok = ok;// & iP5 && iP6 && before;
    //backend->debug();
    return ok;
}

bool testImp12() {
    Backend::SP backend(new Backend(20,20));
    //size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); 
    Imp i2(2,0xF000,impOne); 
    Imp i3(2,0xFF00,impOne); 
    bitset<8> p(rand());
    Imp i4(1,p,0x00,impOne); 
    p = rand();
    Imp i5(2,0x00,p,impOne); 
    p = rand();
    Imp i6(2,p,0x00,impOne); 
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    ImpP iP3 = backend->add(i3);
    ImpP iP4 = backend->add(i4);
    ImpP iP5 = backend->add(i5);
    ImpP iP6 = backend->add(i6);
    ImpReturn iP7 = backend->impUnion(iP5,iP6);
    backend->impIntersection(iP1,iP2);
    backend->impSubtraction(iP3,iP4);
    
    if (iP7.first == SAT ) {
        backend->impUnion(iP7.second,iP5);
    }
    //backend->debug();
    return ok;
}

void testImp() {
    cout << "testImp 1: " << testImp1() << endl;
    cout << "testImp 2: " << testImp2() << endl;
    cout << "testImp 3: " << testImp3() << endl;
    cout << "testImp 4: " << testImp4() << endl;
    cout << "testImp 5: " << testImp5() << endl;
    cout << "testImp 6: " << testImp6() << endl;
    cout << "testImp 7: " << testImp7() << endl;
    cout << "testImp 8: " << testImp8() << endl;
    cout << "testImp 9: " << testImp9() << endl;
    cout << "testImp 10: " << testImp10() << endl;
    cout << "testImp 11: " << testImp11() << endl;
    cout << "testImp 12: " << testImp12() << endl;
}

void testSizes() {
    Level level = 123;
    bitset<16> bits = 0xF00F;
    Imp imp(level,bits,impOne);
    Bdd bdd(level,bddOne,bddOne,impOne,&imp);
    const UpBdd upbdd(&imp,&bdd);
    int a;
    long b=1234123;
    size_t c;
    cout << "size of imp " << sizeof(imp) << endl;
    cout << "size of bdd " << sizeof(bdd) << endl;
    cout << "size of upbdd " << sizeof(upbdd) << endl;
    cout << "size of imp.imp " << sizeof(imp._imp) << endl;
    cout << "size of int " << sizeof(a) << endl;
    cout << "size of long " << sizeof(b) << endl;
    cout << "size of size_t " << sizeof(c) << endl;
    cout << "where points bddOne " << bddOne << endl;
    cout << "where points impOne " << impOne << endl;
}


void testHashFunction() {
    size_t hash = 0; 
    boost::hash_combine(hash,1);
    cout << hash << endl;
    boost::hash_combine(hash,2);
    cout << hash << endl;
    boost::hash_combine(hash,3);
    cout << hash << endl;
    boost::hash_combine(hash,0);
    cout << hash << endl;
}


