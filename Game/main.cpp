#include "SFML/Graphics.hpp"
#include <iostream>


int main() {
    sf::RenderWindow window(sf::VideoMode(720, 480), "3D Game");
    window.setFramerateLimit(60);
    sf::Clock clock;

    sf::CircleShape shape(240.f);
    sf::RectangleShape rect(sf::Vector2 (100.f, 50.f));
    shape.setFillColor(sf::Color::Red);
    rect.setFillColor(sf::Color::Blue);

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        float fps = 1.0f / deltaTime;

        std::cout << fps << "\n";

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        rect.move(120.0f * deltaTime, 60.0f * deltaTime);

        window.clear();
        window.draw(shape);
        window.draw(rect);
        window.display();
    }

    return 0;
}