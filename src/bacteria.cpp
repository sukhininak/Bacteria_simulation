#include "bacteria.h"

// #include <iostream>
// #include <vector>
// #include <cstdlib>
#include <ctime>
Gene::Gene(Gene *gene)
{
        this->gene = gene->gene;
    };
Gene::Gene(){
    size_t size = 5000;
    std::vector<unsigned int> vec(size);
    // Инициализация генератора случайных чисел
    for (size_t i = 0; i < size; ++i) {
        vec[i] = std::rand() % (8);  // числа от 0 до maxValue
    }

    gene = vec;
};

void Gene::Mutate(){
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    gene[std::rand() % (size(gene))] = std::rand() % (size(gene) + 1); 
};

void Gene::ClearGene(){
    for (size_t i = 0; i < size(gene); ++i) {
        gene[i] = 0;
    }
};

void Gene::SetGene(unsigned int position, unsigned int number){
    gene[position % (size(gene))] = number;
};

const std::vector<unsigned int>& Gene::GetGene() const {
    return gene;
};

Group::Group(size_t size){
    std::vector<Bacterium> vec(size);

    for (size_t i = 0; i < size; ++i) { // group info - информация о длине генома, начальной энергии
        vec[i] = Bacterium(i);
    }

    group = vec;
};

Bacterium* Group::GetBacterium(size_t id){
    return &group[id];
};

void Group::UpdateNumberAlive(size_t delta){
    alive += delta;
};

unsigned int Group::GetNumberAlive(){
    return alive;
};
Bacterium::Bacterium(Bacterium *bac, size_t id){
 this->state.energy = bac->state.energy;
    this->state.positionGene = bac->state.positionGene;
    this->state.x = bac->state.x;
    this->state.y = bac->state.y;
    //this->gene = Gene(bac->gene); /////////////////////////////////////////Надо поравить почему-то не работает
    this->ID = id;
};
Bacterium::Bacterium(size_t id){
    state.energy = 20;
    state.positionGene = 0;

    // std::srand(static_cast<unsigned>(std::time(nullptr)));

    // bs.x = std::rand() % (size);
    // bs.y = std::rand() % (size);

    state.x = 0;
    state.y = 0;

    gene = Gene();
    
    ID = id;
    };
Bacterium::Bacterium(int x, int y, size_t id){
    state.energy = 20;
    state.positionGene = 0;

    // std::srand(static_cast<unsigned>(std::time(nullptr)));

    // bs.x = std::rand() % (size);
    // bs.y = std::rand() % (size);

    state.x = x;
    state.y = y;

    gene = Gene();

    ID = id;
};

BacteriumState & Bacterium::GetState(){
    return state;
};

const Gene* Bacterium::GetGene() const{
    return  &gene;
};
size_t Bacterium::GetID(){
    return ID;
};
