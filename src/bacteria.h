#pragma once
#include <vector>
#include <random>

enum class BacteriumType {
    PHOTOSYNTHESIZER = 0, 
    PREDATOR = 1         
};

class Gene {
public:
    Gene();
    Gene(const Gene& other) = default;
    Gene& operator=(const Gene& other) = default;

    void Mutate(std::mt19937& rng);
    const std::vector<unsigned int>& GetGene() const;
    unsigned int GetCommand(unsigned int pos) const;
    unsigned int GetSize() const { return static_cast<unsigned int>(gene.size()); }

private:
    std::vector<unsigned int> gene;
    static constexpr size_t GENE_SIZE = 64;
    static constexpr unsigned int NUM_VALUES = 8;
};

struct BacteriumState {
    int energy = 20;
    unsigned int positionGene = 0;
    int x = 0;
    int y = 0;
    int bacteriumSize = 1; 
};

class Bacterium {
public:
    Bacterium() = default;
    Bacterium(int x, int y, size_t id, BacteriumType type, int bSize = 1);
    Bacterium(const Bacterium& other) = default;
    Bacterium& operator=(const Bacterium& other) = default;

    BacteriumState& GetState();
    const BacteriumState& GetState() const;
    const Gene* GetGene() const;
    Gene* GetGeneMut();
    size_t GetID() const;
    BacteriumType GetType() const;
    void AdvanceGene();

private:
    BacteriumState state;
    Gene gene;
    size_t ID = 0;
    BacteriumType type = BacteriumType::PHOTOSYNTHESIZER;
};
