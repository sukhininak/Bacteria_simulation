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

#include "src/bacteria.h"
#include "src/field.h"
#include "simulator.h"


int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation");
  window.setVerticalSyncEnabled(true);
  ImGui::SFML::Init(window);

  sf::Texture texture;
  texture.create(512, 512);
  sf::Sprite sprite(texture);
  sprite.setPosition((window.getSize().x - texture.getSize().x) >> 1, 80);
  std::vector<sf::Color> pixels(texture.getSize().x * texture.getSize().y);

  std::mutex update_mutex;
  auto show = [&]() mutable {
    std::scoped_lock lock(update_mutex);
    texture.update(reinterpret_cast<uint8_t *>(pixels.data()));
  };

  //   создать симулятор?

  //   Simulator.Process();
  //   Visual.Show();

  // Bacteria bacteria;
  // Field field;

  // for (int i = 0; i < 1000; i++){

  //     // while (bacteria.GetNumberAlive() > 0){
  //     //     for (Bacterium bacterium : bacteria.GetBacteria()){
  //     //         FieldUpdate result = bacterium.DoStep();
  //     //         field.Update(result);
  //     //     };
  //     // }

  //     bacteria.Mutate();
  // }
    

  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::SetWindowFontScale(2.0f);
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

    // Visual.UpdateUI();

    window.clear();
    window.draw(sprite);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}
