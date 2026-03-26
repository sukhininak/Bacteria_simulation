#include "Render.h"

Render::Render(const Field &field):
field(field),
window(sf::VideoMode(field.get_size_W(), field.get_size_H()), "Simulation") 
{ 

    image.create(field.get_size_W(), field.get_size_H(), sf::Color::White);

    // заполняем пиксели
    for (int y = 0; y < field.get_size_W(); y++)
    {
        for (int x = 0; x < field.get_size_H(); x++)
        {
            if (std::holds_alternative<Bacteria>(field.get_cell(x,y).data))
                image.setPixel(x, y, sf::Color::Black);
            else
                image.setPixel(x, y, sf::Color::White);
        }
    }

    texture.loadFromImage(image);

    sprite.setTexture(texture);

}
void Render::update(){
    const int W = field.get_size_W();
    const int H = field.get_size_H();
    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            if (std::holds_alternative<Bacteria>(field.get_cell(x,y).data))
                image.setPixel(x, y, sf::Color::Black);
            else
                image.setPixel(x, y, sf::Color::White);
        }
    }
    texture.update(image);
};
bool Render::is_open() const {
    return window.isOpen();
};
void Render::draw(){
    window.clear();
    window.draw(sprite);
    window.display();
};
bool Render::process_events(sf::Event& event) {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else
            return true;
    }
    return false;
}
;


