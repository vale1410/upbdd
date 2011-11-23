#include "testImp.h" 

#include <iostream>

#include "bddStore.h"
#include "impStore.h"
#include "backend.h"
#include "input.h"

using namespace std;

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

bool testImp13() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawImplication imp;
    imp.push_back(1);
    ImpP iP1 = backend->makeImplication(imp);
    imp.push_back(10);
    ImpP iP2 = backend->makeImplication(imp);
    imp.clear();
    imp.push_back(10);
    ImpP iP3 = backend->makeImplication(imp);
    ImpP output = backend->impSubtraction(iP2,iP1);
    //backend->debug();
    return ok && output == iP3;
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
    cout << "testImp 13: " << testImp13() << endl;
}


