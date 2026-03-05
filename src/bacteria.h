#pragma once
#include <vector>
#include "field.h"

struct BacteriumState{
    int energy;
    unsigned int step;
};

struct Gene{
    std::vector<int> gene;
};

class Bacteria{
public:
    Bacteria();
    std::vector<Bacterium> GetBacteria();
    int GetNumberAlive();
    void Mutate();

private:
    std::vector<Bacterium> bacteria;
};

class Bacterium{
public:
    Bacterium();
    FieldUpdate DoStep();
    void SetState(BacteriumState state);
    BacteriumState GetState();
    void SetGene(Gene gene);
    Gene GetGene();

private:
    BacteriumState state;
    Gene gene; 
};