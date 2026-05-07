#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <algorithm>
#include <mutex>
#include <vector>
#include "simulator.h"
#include "./src/Render.h"

  int main() {
      std::srand(static_cast<unsigned>((std::time(nullptr))));
    Field f(256,256);    

    int id  = 0;

    for (int y = 0; y < 256; ++y) {
    for (int x = 0; x < 256; ++x) {
        bool is_elephant = false;

        // Рисуем геометрические блоки, из которых состоит слон
        
        // Тело (большой прямоугольник в центре)
        if (x >= 70 && x <= 170 && y >= 100 && y <= 160) is_elephant = true;
        
        // Голова (прямоугольник правее и чуть выше)
        if (x > 170 && x <= 210 && y >= 90 && y <= 150) is_elephant = true;
        
        // Хобот (спускается от головы вниз)
        if (x >= 195 && x <= 210 && y > 150 && y <= 210) is_elephant = true;
        
        // Задняя нога
        if (x >= 80 && x <= 100 && y > 160 && y <= 210) is_elephant = true;
        
        // Передняя нога
        if (x >= 140 && x <= 160 && y > 160 && y <= 210) is_elephant = true;
        
        // Хвост (небольшая линия сзади)
        if (x >= 50 && x < 70 && y >= 100 && y <= 110) is_elephant = true;
        
        // Вырез для глаза (делаем этот пиксель пустым, чтобы глаз выделялся)
        if (x >= 185 && x <= 190 && y >= 100 && y <= 105) is_elephant = false;

        // Если текущая координата (x, y) попадает в фигуру, ставим точку
        if (is_elephant) {
            f.add_bac(x, y, id);
            id++;
        }
    }
}
    Render r(f);
    sf::Event event;
  
    Simulator simulator(&f);
    
    while (r.is_open()) {
        r.process_events(event);
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Space)
        {
         simulator.SimulateStep();
         std::cout << "Step: " << simulator.GetStep() << std::endl;
        }
        
        r.update();
        r.draw();
    }
    return 0;
  }
