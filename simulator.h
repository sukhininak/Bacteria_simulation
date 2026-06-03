#pragma once
#include "src/bacteria.h"
#include "src/field.h"

#include <vector>
#include <random>
#include <utility>

struct Context {
    Field* field;
    Bacterium* bacterium;
};

/*
  Кодировка генов:
    0 — Фотосинтез  (+3 энергии, только для PHOTOSYNTHESIZER)
    1,2 — Движение     (следующий ген задаёт направление 0-7)    
    3 — Рост         (увеличить размер, если нет соседей и хватает энергии)
    4 — Поедание     (только для PREDATOR, съесть соседнюю бактерию)
    5 — Деление      (разделиться, если размер >= 2)
 */
enum GeneCommand : unsigned int {
    CMD_PHOTOSYNTHESIS = 0,
    CMD_MOVE1          = 1,
    CMD_MOVE2          = 2,
    CMD_GROW           = 3,
    CMD_EAT            = 4,
    CMD_DIVIDE         = 5,
    CMD_NOP1           = 6,
    CMD_NOP2           = 7,
};

class Simulator {
public:
    Simulator(Field* field);
    unsigned int GetStep() const;
    void SimulateStep();

private:
    void executePhotosynthesis(Bacterium* bac);
    void executeMove(Bacterium* bac, int old_x, int old_y);
    void executeGrow(Bacterium* bac);
    void executeEat(Bacterium* bac);
    void executeDivide(Bacterium* bac);

    bool hasAdjacentBacteria(int x, int y) const;
    std::pair<int,int> findAdjacentEmpty(int x, int y);
    std::pair<int,int> findAdjacentBacterium(int x, int y);

    Field* field;
    unsigned int step;
    std::mt19937 rng;

    static constexpr int dx[8] = { 0,  1,  1,  1,  0, -1, -1, -1};
    static constexpr int dy[8] = {-1, -1,  0,  1,  1,  1,  0, -1};
};
