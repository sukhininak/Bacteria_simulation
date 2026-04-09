#include "simulator.h"

bool Move::Exec(Context& ctx) {
    const Gene* gene = ctx.bacterium->GetGene();
    const unsigned gene_pos = ctx.bacterium->GetState().positionGene;
    unsigned param_pos = gene_pos + 1;
    if (param_pos >= gene->GetGene().size()) {
        param_pos = 0;
    }
    const unsigned param = gene->GetGene().at(param_pos) % 8;

    unsigned new_x = ctx.bacterium->GetState().x;
    unsigned new_y = ctx.bacterium->GetState().y;
    switch(param) {
        case 0:
          new_y --;
          break;
        case 1:
          new_x ++;
          new_y --;
          break;
        case 2:
          new_x ++;
          break;
        case 3: 
          new_x ++;
          new_y ++;
          break;
        case 4:
          new_y ++;
          break;
        case 5:
          new_y ++;
          new_x --;
        case 6:
          new_x --;
          break;
        case 7:
          new_x --;
          new_y --;
          break;
    }
if(new_x < ctx.field->get_size_W() && new_y < ctx.field->get_size_H() && new_x >= 0 && new_y >= 0)
    if (ctx.field->get_cell(new_x, new_y).IsEmpty()) {
           ctx.field->add_bac(new_x, new_y, ctx.bacterium->GetID());
        ctx.field->delete_bac(ctx.bacterium->GetState().x, ctx.bacterium->GetState().y);
        return true;
    }

    return false;
}

Simulator::Simulator(Field* field) : field(field), step(0) {}
   
unsigned int Simulator::GetStep() const { return step; }

void Simulator::SimulateStep() {
    step++;
    std::vector<Bacterium*> bacteries;
    for (unsigned x = 0; x < field->get_size_W(); ++x) {
        for (unsigned y = 0; y < field->get_size_H(); ++y) {
            auto& cell = field->get_cell(x, y);
            if (cell.GetCode() == 1) {
                bacteries.push_back(cell.as<Bacterium>());
            }
        }
    }

    for (auto* bac : bacteries) {
        if(bac->GetGene()->GetGene()[step] == 5) {
            Context ctx{field, bac};
            Move move;
            move.Exec(ctx);
        }
    }
}