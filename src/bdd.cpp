#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

class Imp;

class Bdd {
    public:
        Bdd() :
        level(0) ,
        high(NULL)
        {
            
        }

        const unsigned int level;

        const Bdd* high;
        const Imp* hImp;
        const Bdd* low;
        const Imp* lImp;
        
        std::string toString() {
            std::ostringstream oss;
            oss << this << " = ";
            oss << "(";
            oss << level;
            oss <<  ",";
            oss << high;
            oss <<  ",";
            oss << hImp;
            oss <<  ",";
            oss << low;
            oss <<  ",";
            oss << lImp;
            oss <<  ")";
            return oss.str();
        }
};


class Imp {
    public:
        Imp():
        level(0),
        implications(431876543),
        next(NULL)
        {}

        const unsigned int level;
        // bit 0 - 15 positive, bit 16 - 31 negative
        const int implications;
        const Imp* next;
        
        std::string toString() {
            std::ostringstream oss;
            oss << this << " = ";
            oss << "(";
            oss << level;
            oss <<  ",";
            oss << implications;
            oss <<  ",";
            oss << next;
            oss <<  ")";
            return oss.str();
        }
};


void printImp(Imp imp) 
{
    std::cout << "imp: " << imp.toString() << std::endl;
}

void printBdd(Bdd& bdd) 
{
    std::cout << "bdd: " << bdd.toString() << std::endl;
}


int main () 
{
    Bdd bdd;
    Imp imp;
    std::cout << "first tests for bdds" << std::endl;
    printBdd(bdd);
    printImp(imp);
    return 0; 
} 

