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

    for (int i = 0; i < 256; ++i) {
        f.add_bac(i,
                     i,  id);
        id++;
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
