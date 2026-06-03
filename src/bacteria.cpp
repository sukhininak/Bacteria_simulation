#include "bacteria.h"

static std::mt19937& getGlobalRNG() {
    static std::mt19937 rng(std::random_device{}());
    return rng;
}


Gene::Gene() {
    gene.resize(GENE_SIZE);
    std::uniform_int_distribution<unsigned int> dist(0, NUM_VALUES - 1);
    auto& rng = getGlobalRNG();
    for (size_t i = 0; i < GENE_SIZE; ++i) {
        gene[i] = dist(rng);
    }
}

void Gene::Mutate(std::mt19937& rng) {
    std::uniform_int_distribution<size_t> posDist(0, gene.size() - 1);
    std::uniform_int_distribution<unsigned int> valDist(0, NUM_VALUES - 1);
    gene[posDist(rng)] = valDist(rng);
}

const std::vector<unsigned int>& Gene::GetGene() const {
    return gene;
}

unsigned int Gene::GetCommand(unsigned int pos) const {
    return gene[pos % gene.size()];
}

Bacterium::Bacterium(int x, int y, size_t id, BacteriumType type, int bSize)
    : ID(id), type(type)
{
    state.x = x;
    state.y = y;
    state.energy = 200;
    state.positionGene = 0;
    state.bacteriumSize = bSize;
    gene = Gene();
}

BacteriumState& Bacterium::GetState() { return state; }
const BacteriumState& Bacterium::GetState() const { return state; }
const Gene* Bacterium::GetGene() const { return &gene; }
Gene* Bacterium::GetGeneMut() { return &gene; }
size_t Bacterium::GetID() const { return ID; }
BacteriumType Bacterium::GetType() const { return type; }

void Bacterium::AdvanceGene() {
    state.positionGene = (state.positionGene + 1) % gene.GetSize();
}
