#include "bacteria.h"

// #include <iostream>
// #include <vector>
// #include <cstdlib>
#include <ctime>


Gene::Gene(size_t size){
    std::vector<unsigned int> vec(size);
    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (size_t i = 0; i < size; ++i) {
        vec[i] = std::rand() % (size + 1);  // числа от 0 до maxValue
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

std::vector<unsigned int>* Gene::GetGene(){
    return &gene;
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


Bacterium::Bacterium(size_t id){
    BacteriumState bs;

    bs.energy = 20;
    bs.positionGene = 0;

    // std::srand(static_cast<unsigned>(std::time(nullptr)));

    // bs.x = std::rand() % (size);
    // bs.y = std::rand() % (size);

    bs.x = 0;
    bs.y = 0;

    state = bs;

    gene = Gene(20);

    ID = id;
};

BacteriumState* Bacterium::GetState(){
    return &state;
};

const Gene* Bacterium::GetGene() const{
    return  &gene;
};