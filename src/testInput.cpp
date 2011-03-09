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
        foreach(Literal variable, clause) {
                cout << variable << " ";
        };
        cout << endl;
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

UpBdd testInput4help(Backend::SP backend, RawProblem problem) {
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
    cout << "4_1\n";
    UpBdd up1 = testInput4help(backend, p1);
    cout << "4_2\n";
    //UpBdd up2 = testInput4help(backend, p2);
    cout << "4_3\n";
    UpBdd up3 = testInput4help(backend, p3);
    cout << "4_4\n";
    //UpBdd up4 = testInput4help(backend, p4);
    ok = ok && up1 == up3; //&& up2 == up3 && up3 == up4;
    backend->debug(); 
    return ok;
}


void testInput(){
    //cout << "testInput 1: " << testInput1() << endl;
    //cout << "testInput 2: " << testInput2() << endl;
    //cout << "testInput 3: " << testInput3() << endl;
    cout << "testInput 4: " << testInput4() << endl;
}
