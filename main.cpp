#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <iostream>
#include <random>
#include <string>

#include "simulator.h"
#include "src/render.h"
#include "patterns.h"

int main() {
    std::mt19937 rng(std::random_device{}());

    Field f(256, 256);

    generatePattern(f, PatternType::FACE, rng);

    Render r(f);
    Simulator simulator(&f);

    while (r.is_open()) {
        r.process_events();

        if (r.wantsReset()) {
            r.clearReset();
            int sel = r.getSelectedPattern();
            generatePattern(f, static_cast<PatternType>(sel), rng);
            simulator = Simulator(&f);
        }

        bool shouldSimulate = false;

        if (!r.isPaused()) {
            shouldSimulate = true;
        }

        if (r.wantsSingleStep()) {
            shouldSimulate = true;
            r.clearSingleStep();
        }

        if (shouldSimulate) {
            simulator.SimulateStep();
        }

        int totalCount = 0;
        int greenCount = 0;
        int redCount   = 0;
        for (int x = 0; x < f.get_size_W(); ++x) {
            for (int y = 0; y < f.get_size_H(); ++y) {
                const auto& cell = f.get_cell(x, y);
                if (cell.GetCode() == 1) {
                    const Bacterium* bac = cell.as<Bacterium>();
                    if (bac) {
                        totalCount++;
                        if (bac->GetType() == BacteriumType::PHOTOSYNTHESIZER)
                            greenCount++;
                        else
                            redCount++;
                    }
                }
            }
        }

        r.update(simulator.GetStep(), totalCount, greenCount, redCount);
        r.draw();
    }

    return 0;
}
