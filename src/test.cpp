#include <iostream>

#include "bddStore.h"
#include "impStore.h"

void testSizes();
void testImp();
void testBdd();
void testHashFunction();

int main () 
{
    //testSizes();
    //testImp();
    testBdd();
    //testHashFunction();
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
    UpBdd upbdd2 = backend->add(bdd2);
    backend->debug();
    bool ok = upbdd1._bddP == bddOne && upbdd1._impP == iP1;
    return ok && before + 1 == backend->sizeBdd();
}

bool testBdd2() {
    Backend::SP backend(new Backend(20,20));
    size_t before = backend->sizeBdd();
    Imp i1(1,0x0100,impOne); // 1
    Imp i2(1,0x0001,impOne); // -1
    ImpP iP1 = backend->add(i1);
    ImpP iP2 = backend->add(i2);
    Bdd bdd1(2,bddOne,bddOne,iP1,iP2); // 2 -> 1 ; -1
    Bdd bdd2(2,bddOne,bddOne,iP2,iP1); // 2 -> -1 ; 1
    UpBdd upBdd1 = backend->add(bdd1);
    UpBdd upBdd2 = backend->add(bdd2);
    Bdd bdd3(3,upBdd1,upBdd2); // 3 -> bdd1 ; bdd2
    backend->add(bdd3);
    backend->debug();
    return before + 3 == backend->sizeBdd();
}


void testBdd() {
    //std::cout << "testBdd 1: " << testBdd1() << std::endl;
    std::cout << "testBdd 2: " << testBdd2() << std::endl;
    //std::cout << "testBdd 3: " << testBdd3() << std::endl;
    //std::cout << "testBdd 4: " << testBdd4() << std::endl;
    //std::cout << "testBdd 5: " << testBdd5() << std::endl;
    //std::cout << "testBdd 6: " << testBdd6() << std::endl;
    //std::cout << "testBdd 7: " << testBdd6() << std::endl;
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
    Imp i2(1,0x0010,impOne); // 1
    Imp i3(1,0x0110,impOne); // 1
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
    Imp i2(1,0x0001,impOne); // 1
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
    Imp i2(2,0x0001,impOne); // 1
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
    Imp i2(2,0xF000,impOne); // 1
    Imp i3(2,0x000F,impOne); // 1
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
    backend->debug();
    return ok;
}

bool testImp12() {
    Backend::SP backend(new Backend(20,20));
    //size_t before = backend->sizeImp();
    bool ok = true;
    Imp i1(1,0x0100,impOne); 
    Imp i2(2,0xF000,impOne); 
    Imp i3(2,0xFF00,impOne); 
    Imp i4(1,0x0000,impOne); 
    Imp i5(2,0x0000,impOne); 
    Imp i6(2,0x0000,impOne); 
    std::bitset<8> p(std::rand());
    i4.setPos(p);
    p = std::rand();
    i5.setNeg(p);
    p = std::rand();
    i6.setPos(p);
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
    backend->debug();
    return ok;
}

void testImp() {
    std::cout << "testImp 1: " << testImp1() << std::endl;
    std::cout << "testImp 2: " << testImp2() << std::endl;
    std::cout << "testImp 3: " << testImp3() << std::endl;
    std::cout << "testImp 4: " << testImp4() << std::endl;
    std::cout << "testImp 5: " << testImp5() << std::endl;
    std::cout << "testImp 6: " << testImp6() << std::endl;
    std::cout << "testImp 7: " << testImp7() << std::endl;
    std::cout << "testImp 8: " << testImp8() << std::endl;
    std::cout << "testImp 9: " << testImp9() << std::endl;
    std::cout << "testImp 10: " << testImp10() << std::endl;
    std::cout << "testImp 11: " << testImp11() << std::endl;
    std::cout << "testImp 12: " << testImp12() << std::endl;
}

void testSizes() {
    Level level = 123;
    std::bitset<16> bits = 0xF00F;
    Imp imp(level,bits,impOne);
    Bdd bdd(level,bddOne,bddOne,impOne,&imp);
    const UpBdd upbdd(&imp,&bdd);
    int a;
    long b=1234123;
    size_t c;
    std::cout << "size of imp " << sizeof(imp) << std::endl;
    std::cout << "size of bdd " << sizeof(bdd) << std::endl;
    std::cout << "size of upbdd " << sizeof(upbdd) << std::endl;
    std::cout << "size of imp.imp " << sizeof(imp._imp) << std::endl;
    std::cout << "size of int " << sizeof(a) << std::endl;
    std::cout << "size of long " << sizeof(b) << std::endl;
    std::cout << "size of size_t " << sizeof(c) << std::endl;
    std::cout << "where points bddOne " << bddOne << std::endl;
    std::cout << "where points impOne " << impOne << std::endl;
}


void testHashFunction() {
    size_t hash = 0; 
    boost::hash_combine(hash,1);
    std::cout << hash << std::endl;
    boost::hash_combine(hash,2);
    std::cout << hash << std::endl;
    boost::hash_combine(hash,3);
    std::cout << hash << std::endl;
    boost::hash_combine(hash,0);
    std::cout << hash << std::endl;
}


