#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <bitset>
#include <math.h>

template<int N>
class Imp;

template<int M,int N>
class Bdd {
    public:
    Bdd(unsigned int level,Imp<N>* hImp,Imp<N>* lImp) :
        _level(level),
        _function(0x0F0F),
        _high(NULL),
        _hImp(hImp),
        _low(NULL),
        _lImp(lImp)
        {
            
        }

        const unsigned int _level;
        std::bitset<M> _function;
        
        const Bdd<M,N>* _high;
        const Imp<N>* _hImp;
        const Bdd<M,N>* _low;
        const Imp<N>* _lImp;
        
        std::string toString() {
            std::ostringstream oss;
            oss << this << " = ";
            oss << "(";
            oss << _level;
            oss <<  ",";
            oss << _function;
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

template<int N>
class Imp {
    public:
        Imp():
        _level(0),
        _imp(0x0F0F01),
        next(NULL)
        {}

        const unsigned int _level;
        const std::bitset<2 * N> _imp;
        const Imp<N>* next;
        
        std::string toString() {
            std::ostringstream oss;
            oss << this << " = ";
            oss << "(";
            oss << _level;
            oss <<  ",";
            oss << _imp;
            oss <<  ",";
            //oss << neg;
            //oss <<  ",";
            oss << next;
            oss <<  ")";
            return oss.str();
        }
};

template<int N>                 
void printImp(Imp<N>& imp) 
{
    std::cout << "imp: " << imp.toString() << std::endl;
}

template<int M,int N>                 
void printBdd(Bdd<M,N>& bdd) 
{
    std::cout << "bdd: " << bdd.toString() << std::endl;
}


int main () 
{
    unsigned int level = 5;
    const unsigned int size = 4;
    const unsigned int sizeBdd = 24;//pow(2,4);
    std::cout << sizeBdd << std::endl;
    Imp<size> imp;
    Bdd<sizeBdd,size> bdd(level,&imp,&imp);
    std::cout << "first tests for bdds" << std::endl;
    printImp(imp);
    printBdd(bdd);
    int a;
    long b;
    size_t c;
    std::cout << "size of bdd " << sizeof(bdd) << std::endl;
    std::cout << "size of imp " << sizeof(imp) << std::endl;
    std::cout << "size of imp.imp " << sizeof(imp._imp) << std::endl;
    std::cout << "size of function " << sizeof(bdd._function) << std::endl;
    std::cout << "size of int " << sizeof(a) << std::endl;
    std::cout << "size of long " << sizeof(b) << std::endl;
    std::cout << "size of size_t " << sizeof(c) << std::endl;
} 

