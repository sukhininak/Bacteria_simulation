# pragma once
#include "field.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include <algorithm>
#include <mutex>

class Render{
    
    public:
    Render( const Field  &field);
    void update();
    bool is_open() const;
    void draw();
    bool process_events(sf::Event& event);

    private: 
    const Field &field;

    sf::RenderWindow window;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
};