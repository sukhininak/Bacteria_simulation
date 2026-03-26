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
  f.add_bac(10,10);
  f.add_bac(11,11);
  f.add_bac(12,12);
  
  Render r(f);
  return 0;
}
