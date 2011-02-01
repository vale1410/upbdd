#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

class Imp;

class Bdd {
    public:
        Bdd(unsigned int level,Imp* hImp,Imp* lImp) :
        _level(level),
        _high(NULL),
        _hImp(hImp),
        _low(NULL),
        _lImp(lImp)
        {
            
        }

        const unsigned int _level;

        const Bdd* _high;
        const Imp* _hImp;
        const Bdd* _low;
        const Imp* _lImp;
        
        std::string toString() {
            std::ostringstream oss;
            oss << this << " = ";
            oss << "(";
            oss << _level;
            oss <<  ",";
            oss << _high;
            oss <<  ",";
            oss << _hImp;
            oss <<  ",";
            oss << _low;
            oss <<  ",";
            oss << _lImp;
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
    unsigned int level = 5;
    Imp imp;
    Bdd bdd(level,&imp,&imp);
    std::cout << "first tests for bdds" << std::endl;
    printImp(imp);
    printBdd(bdd);

    std::cout << "size of bdd " << sizeof(bdd) << std::endl;
    std::cout << "size of imp " << sizeof(imp) << std::endl;
    return 0; 
} 

