#pragma once
#include "field.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

#include <string>

class Render {
public:
    Render(const Field& field);
    ~Render();

    void update(unsigned int step, int totalCount, int greenCount, int redCount);
    bool is_open() const;
    void draw();
    void process_events();

    bool isPaused() const { return paused; }
    bool wantsSingleStep() const { return singleStep; }
    void clearSingleStep() { singleStep = false; }
    void savePNG(const std::string& filename);

    int  getSelectedPattern() const { return selectedPattern; }
    bool wantsReset() const { return resetRequested; }
    void clearReset() { resetRequested = false; }

private:
    void updateFieldTexture();
    void drawUI();

    const Field& field;
    sf::RenderWindow window;
    sf::RenderTexture fieldRT;
    sf::Clock deltaClock;

    bool paused = true;
    bool singleStep = false;
    unsigned int currentStep = 0;
    int totalBacteria = 0;
    int greenBacteria = 0;
    int redBacteria = 0;

    int selectedPattern = 0;
    bool resetRequested = false;

    static constexpr int CELL_SIZE = 3;
    static constexpr int UI_PANEL_WIDTH = 260;
};
