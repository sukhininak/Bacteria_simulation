#pragma once
#include <vector>

class Gene{
public:
    Gene();
    Gene(size_t size);

    void Mutate();

    void ClearGene();
    void SetGene(unsigned int position, unsigned int number);

    std::vector<unsigned int>* GetGene();

private:
    std::vector<unsigned int> gene;
};

class Group{
public:
    Group(size_t size);

    Bacterium* GetBacterium(size_t id); //delete bacterium?

    void UpdateNumberAlive(size_t delta);
    unsigned int GetNumberAlive();

private:
    std::vector<Bacterium> group;
    unsigned int alive;
};

struct BacteriumState{
    int energy;
    unsigned int positionGene;
    unsigned int x;
    unsigned int y;
};

class Bacterium{
public:
    Bacterium();
    Bacterium(size_t id);// бактерия родилась на поле или мы новую бактерию садим в клетку
    
    //virtual machine

    BacteriumState* GetState();
    const Gene* GetGene() const; //const? нельзя менять ген

private:
    BacteriumState state;
    Gene gene; 
    size_t ID;
};