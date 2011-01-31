#include <iostream>

class Bdd {
    public:
        Bdd()
        :level(0) 
        {}

        unsigned int level;
        Bdd* high;
        Bdd* low;
};


class Imp {
    public:
        Imp()
        :level(0) 
        {}

        unsigned int level;
        Bdd* next;
        Bdd* low;
};

size_t hash(Bdd& bdd) 
{
    std::cout << bdd.high << std::endl;
    std::cout << bdd.low << std::endl;
    return bdd.level;
}


int main () 
{
    Bdd bdd;
    std::cout << "first tests for bdds" << std::endl;
    std::cout << hash(bdd) << std::endl;
    return 0; 
} 

