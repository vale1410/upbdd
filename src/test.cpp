#include <iostream>

#include "bddStore.h"
#include "impStore.h"
#include "backend.h"
#include "input.h"

#include "testBdd.h"
#include "testImp.h"
#include "testInput.h"
#include "testProblem.h"


using namespace std;

void testSizes();
void testHashFunction();

int main () 
{
    //testSizes();
    testInput();
    //testImp();
    //testBdd(); //testHashFunction();
    return 0;
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


