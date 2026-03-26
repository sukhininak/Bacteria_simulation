#include "Render.h"

Render::Render(const Field &field):field(field) { 
    const int W = field.get_size_W();
    const int H = field.get_size_H();
    sf::RenderWindow window(sf::VideoMode(W, H), "Simulation");

    sf::Image image;
    image.create(W, H, sf::Color::White);

    // заполняем пиксели
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

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite(texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}