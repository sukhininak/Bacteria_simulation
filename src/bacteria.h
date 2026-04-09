#pragma once
#include <vector>

class Gene{
public:
    Gene() = default;
    Gene(size_t size);

    void Mutate();

    void ClearGene();
    void SetGene(unsigned int position, unsigned int number);

    const std::vector<unsigned int>& GetGene() const;

private:
    std::vector<unsigned int> gene;
};

struct BacteriumState{
    int energy;
    unsigned int positionGene;
    unsigned int x;
    unsigned int y;
};

class Bacterium{
public:
    Bacterium(int x, int y,size_t id);// бактерия родилась на поле или мы новую бактерию садим в клетку
    Bacterium(size_t id);
    Bacterium()= default;
    //virtual machine

    BacteriumState &GetState();
    const Gene* GetGene() const; //const? нельзя менять ген
    size_t GetID();
private:
    BacteriumState state;
    Gene gene; 
    size_t ID;
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