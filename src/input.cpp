#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "input.h"

using namespace std;

void parseProblem(string file, RawProblem& problem) {
    ifstream in(file.c_str());
    if (in.is_open()) {
        //cout << "c file opened" << endl;
    } else {
        //cout << "c file not found" << endl;
    }
    string str;
    typedef boost::tokenizer<boost::char_separator<char> > Tok;
    boost::char_separator<char> sep(" "); 
    
    getline(in, str);
    Tok tok(str,sep);
    while(!in.fail())
    {
        //cout << "c reading: " << str << endl;                                
        tok = Tok(str,sep);

        Clause clause;
        bool is_clause = true;
        for(Tok::iterator beg=tok.begin(); is_clause && beg!=tok.end();++beg){
            if (*beg != "c") {
                int var = boost::lexical_cast<Literal>(*beg);
                if (var != 0) clause.push_back(var);
            } else {
                is_clause = false;
            }
        }
        if (is_clause) {
            is_clause = true;
            problem.push_back(clause);
            clause.clear();
        }
        getline(in, str);
    }
    in.close();
    //cout << "c finished parsing!" << endl << endl;
    cout << endl;
}
