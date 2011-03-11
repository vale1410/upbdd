#include <iostream>

#include "bddStore.h"
#include "impStore.h"
#include "backend.h"
#include "input.h"


using namespace std;

bool testInput1() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem problem; 
    parseProblem("data/testInput1.cnf", problem);
    //cout << "c what did I read: " << endl;
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
        UpBdd tmp = backend->makeClause(clause);
        BddReturn result = backend->bddAnd(up,tmp);
        //cout << "bddAnd(" << up.toString() << ", " << tmp.toString();
        if (result.first == SAT) {
            //cout << ") = (SAT, " << result.second.toString() << ")\n";
            up = result.second;
        } else {
            //cout << ") =  UNSAT" << endl;
        }
        if (has_results[i]) {
            ok = ok && results[i] == result;
        }
        i++;
   };
   //backend->debug(); 
   return ok;
}

bool testInput2() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem problem; 
    parseProblem("data/testInput2.cnf", problem);
    //cout << "c what did I read: " << endl;
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
        UpBdd tmp = backend->makeClause(clause);
        BddReturn result = backend->bddAnd(up,tmp);
        //cout << "bddAnd(" << up.toString() << ", " << tmp.toString();
        if (result.first == SAT) {
            //cout << ") = (SAT, " << result.second.toString() << ")\n";
            up = result.second;
        } else {
            //cout << ") =  UNSAT" << endl;
        }
        if (has_results[i]) {
            ok = ok && results[i] == result;
        }
        i++;
   };
   //backend->debug(); 
   return ok;
}

bool testInput3() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem problem; 
    parseProblem("data/testInput3.cnf", problem);
    //cout << "c what did I read: " << endl;
    UpBdd up;
    vector<bool> has_results;
    vector<BddReturn> results;
    
    foreach(Clause clause, problem) {
        UpBdd tmp = backend->makeClause(clause);
        BddReturn result = backend->bddAnd(up,tmp);
        //cout << "bddAnd(" << up.toString() << ", " << tmp.toString();
        //if (result.first == SAT) {
        //    cout << ") = (SAT, " << result.second.toString() << ")\n";
        //    up = result.second;
        //} else {
        //    cout << ") =  UNSAT" << endl;
        //}
   };
   //backend->debug(); 
   return ok;
}

UpBdd conjunctRawProblem(Backend::SP backend, RawProblem problem) {
    UpBdd up;
    foreach(Clause clause, problem) {
        UpBdd tmp = backend->makeClause(clause);
        BddReturn result = backend->bddAnd(up,tmp);
        cout << "bddAnd(" << up.toString() << ", " << tmp.toString();
        if (result.first == SAT) {
            cout << ") = (SAT, " << result.second.toString() << ")\n";
        up = result.second;
        } else {
            cout << ") =  UNSAT" << endl;
        }
        backend->printSize();
   };
   return up;
}

bool testInput4() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem p1; 
    RawProblem p2; 
    RawProblem p3; 
    RawProblem p4; 
    parseProblem("data/testInput4_1.cnf", p1);
    parseProblem("data/testInput4_2.cnf", p2);
    parseProblem("data/testInput4_3.cnf", p3);
    parseProblem("data/testInput4_4.cnf", p4);
    //cout << "4_1\n";
    UpBdd up1 = conjunctRawProblem(backend, p1);
    //cout << "4_2\n";
    UpBdd up2 = conjunctRawProblem(backend, p2);
    //cout << "4_3\n";
    UpBdd up3 = conjunctRawProblem(backend, p3);
    //cout << "4_4\n";
    UpBdd up4 = conjunctRawProblem(backend, p4);
    ok = ok && up1 == up3 && up2 == up3 && up3 == up4;
    //backend->debug(); 
    return ok;
}
    
bool testInput5() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem p1; 
    RawProblem p2; 
    RawProblem p3; 
    RawProblem p4; 
    parseProblem("data/testInput5_1.cnf", p1);
    parseProblem("data/testInput5_2.cnf", p2);
    parseProblem("data/testInput5_3.cnf", p3);
    parseProblem("data/testInput5_4.cnf", p4);
    //cout << "5_1\n";
    UpBdd up1 = conjunctRawProblem(backend, p1);
    //cout << "5_2\n";
    UpBdd up2 = conjunctRawProblem(backend, p2);
    //cout << "5_3\n";
    UpBdd up3 = conjunctRawProblem(backend, p3);
    //cout << "5_4\n";
    UpBdd up4 = conjunctRawProblem(backend, p4);
    ok = ok && up1 == up2 && up2 == up3 && up3 == up4;
    //backend->debug(); 
    return ok;
}

bool testInput6() {
    bool ok = true;
    Backend::SP backend(new Backend(20,20));
    RawProblem problem; 
    parseProblem("data/testInput6.cnf", problem);
    cout << "c what did I read: " << endl;
    //printProblem(problem);
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
        UpBdd tmp = backend->makeClause(clause);
        BddReturn result = backend->bddAnd(up,tmp);
        //cout << "bddAnd(" << up.toString() << ", " << tmp.toString();
        if (result.first == SAT) {
            //cout << ") = (SAT, " << result.second.toString() << ")\n";
            up = result.second;
        } else {
            //cout << ") =  UNSAT" << endl;
        }
        if (has_results[i]) {
            ok = ok && results[i] == result;
        }
        i++;
   };
   //backend->debug(); 
   return ok;
}

bool testInput7() {
    bool ok = true;
    Backend::SP backend(new Backend(1000000,1000000));
    RawProblem p; 
    parseProblem("data/counting-easier-php-012-010.sat05-1172.reshuffled-07.cnf", p);
    //printProblem(p);
    sort(p.begin(),p.end(),ClauseOrder());
    //printProblem(p);
    UpBdd up = conjunctRawProblem(backend, p);
    backend->printSize();
    cout << " upbdd: " << up.toString() << endl;
    //backend->debug(); 
    return ok;
}

void testInput(){
    //cout << "testInput 1: " << testInput1() << endl;
    //cout << "testInput 2: " << testInput2() << endl;
    //cout << "testInput 3: " << testInput3() << endl;
    //cout << "testInput 4: " << testInput4() << endl;
    //cout << "testInput 5: " << testInput5() << endl;
    //cout << "testInput 6: " << testInput6() << endl;
    cout << "testInput 7: " << testInput7() << endl;
}
