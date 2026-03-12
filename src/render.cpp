#include "render.h"

Render::Render(){ const int W = 256;
    const int H = 256;

    sf::RenderWindow window(sf::VideoMode(W, H), "256x256");

    sf::Image image;
    image.create(W, H, sf::Color::White);

    // заполняем пиксели
    for (int y = 0; y < H; y++)
    {
        for (int x = 0; x < W; x++)
        {
            if ((x + y) % 2 == 0)
                image.setPixel(x, y, sf::Color::White);
            else
                image.setPixel(x, y, sf::Color::Black);
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