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

#include "./src/Render.h"

  int main() {
    Field f(256,256);    
    Render r(f);
    sf::Event event;



   
    while (r.is_open()) {
        r.process_events(event);
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Space)
        {
        f.add_bac(rand()%f.get_size_W(),
                  rand()%f.get_size_H());
        }
        
        r.update();
        r.draw();
    }
    return 0;
  }
