#include "simulator.h"
#include <algorithm>

Simulator::Simulator(Field* field)
    : field(field), step(0), rng(std::random_device{}())
{}

unsigned int Simulator::GetStep() const { return step; }

void Simulator::SimulateStep() {
    step++;

    std::vector<std::pair<int,int>> positions;
    positions.reserve(1024);
    for (int x = 0; x < field->get_size_W(); ++x) {
        for (int y = 0; y < field->get_size_H(); ++y) {
            if (field->get_cell(x, y).GetCode() == 1) {
                positions.push_back({x, y});
            }
        }
    }

    std::shuffle(positions.begin(), positions.end(), rng);

    for (auto [bx, by] : positions) {
        auto& cell = field->get_cell(bx, by);
        if (cell.GetCode() != 1) continue; 

        Bacterium* bac = cell.as<Bacterium>();
        if (!bac) continue;

        bac->GetState().energy -= 1;
        if (bac->GetState().energy <= 0) {
            field->delete_bac(bx, by);
            continue;
        }

        unsigned int genePos = bac->GetState().positionGene;
        unsigned int cmd = bac->GetGene()->GetCommand(genePos);
        bac->AdvanceGene();

        switch (cmd) {
            case CMD_PHOTOSYNTHESIS:
                executePhotosynthesis(bac);
                break;

            case CMD_MOVE1:
            case CMD_MOVE2:
                executeMove(bac, bx, by);
                break;

            case CMD_GROW:
                executeGrow(bac);
                break;

            case CMD_EAT:
                executeEat(bac);
                break;

            case CMD_DIVIDE:
                executeDivide(bac);
                break;

            default:
                break; 
        }
    }
}

void Simulator::executePhotosynthesis(Bacterium* bac) {
    if (bac->GetType() == BacteriumType::PHOTOSYNTHESIZER) {
        bac->GetState().energy += 3;
    }
}

void Simulator::executeMove(Bacterium* bac, int old_x, int old_y) {
    unsigned int genePos = bac->GetState().positionGene;
    unsigned int dir = bac->GetGene()->GetCommand(genePos) % 8;
    bac->AdvanceGene();

    int new_x = old_x + dx[dir];
    int new_y = old_y + dy[dir];

    if (new_x < 0 || new_x >= field->get_size_W() ||
        new_y < 0 || new_y >= field->get_size_H() ||
        !field->get_cell(new_x, new_y).IsEmpty())
    {
        return; 
    }

    Bacterium copy = *bac;
    copy.GetState().energy -= 1; 

    if (copy.GetState().energy <= 0) {
        field->delete_bac(old_x, old_y);
        return;
    }

    copy.GetState().x = new_x;
    copy.GetState().y = new_y;

    field->delete_bac(old_x, old_y);
    field->set_cell(new_x, new_y, std::move(copy));
}

void Simulator::executeGrow(Bacterium* bac) {
    if (bac->GetState().bacteriumSize >= 3) return; 
    if (bac->GetState().energy < 10) return;        

    int x = bac->GetState().x;
    int y = bac->GetState().y;

    if (hasAdjacentBacteria(x, y)) return; 

    bac->GetState().energy -= 10;
    bac->GetState().bacteriumSize += 1;
}

void Simulator::executeEat(Bacterium* bac) {
    if (bac->GetType() != BacteriumType::PREDATOR) return; 

    int x = bac->GetState().x;
    int y = bac->GetState().y;

    auto [tx, ty] = findAdjacentBacterium(x, y);
    if (tx < 0) return;

    Bacterium* prey = field->get_cell(tx, ty).as<Bacterium>();
    if (prey) {
        bac->GetState().energy += prey->GetState().energy;
        field->delete_bac(tx, ty);
    }
}

void Simulator::executeDivide(Bacterium* bac) {
    if (bac->GetState().bacteriumSize < 2) return; 
    if (bac->GetState().energy < 5) return;       

    int x = bac->GetState().x;
    int y = bac->GetState().y;

    auto [nx, ny] = findAdjacentEmpty(x, y);
    if (nx < 0) return; 

    int newSize    = bac->GetState().bacteriumSize - 1;
    int halfEnergy = (bac->GetState().energy - 5) / 2;

    bac->GetState().bacteriumSize = newSize;
    bac->GetState().energy        = halfEnergy;

    size_t childId = field->getNextId();
    Bacterium child(nx, ny, childId, bac->GetType(), newSize);
    child.GetState().energy = halfEnergy;

    child.GetGeneMut()->Mutate(rng);

    field->set_cell(nx, ny, std::move(child));
}


bool Simulator::hasAdjacentBacteria(int x, int y) const {
    for (int d = 0; d < 8; ++d) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < field->get_size_W() &&
            ny >= 0 && ny < field->get_size_H())
        {
            if (field->get_cell(nx, ny).GetCode() == 1) return true;
        }
    }
    return false;
}

std::pair<int,int> Simulator::findAdjacentEmpty(int x, int y) {
    std::uniform_int_distribution<int> dist(0, 7);
    int start = dist(rng);
    for (int i = 0; i < 8; ++i) {
        int d  = (start + i) % 8;
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < field->get_size_W() &&
            ny >= 0 && ny < field->get_size_H())
        {
            if (field->get_cell(nx, ny).IsEmpty()) return {nx, ny};
        }
    }
    return {-1, -1};
}

std::pair<int,int> Simulator::findAdjacentBacterium(int x, int y) {
    std::uniform_int_distribution<int> dist(0, 7);
    int start = dist(rng);
    for (int i = 0; i < 8; ++i) {
        int d  = (start + i) % 8;
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < field->get_size_W() &&
            ny >= 0 && ny < field->get_size_H())
        {
            if (field->get_cell(nx, ny).GetCode() == 1) return {nx, ny};
        }
    }
    return {-1, -1};
}
