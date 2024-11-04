#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

class Balloon {
public:
    Balloon(const sf::Vector2u& windowSize) {
        shape.setRadius(20.f);
        shape.setFillColor(sf::Color::Red); 

       
        int side = rand() % 4;
        switch (side) {
        case 0: 
            shape.setPosition(0, rand() % windowSize.y);
            break;
        case 1:
            shape.setPosition(windowSize.x, rand() % windowSize.y);
            break;
        case 2: 
            shape.setPosition(rand() % windowSize.x, 0);
            break;
        case 3: 
            shape.setPosition(rand() % windowSize.x, windowSize.y);
            break;
        }

       
        targetPosition = sf::Vector2f(windowSize.x / 2.f, windowSize.y / 2.f);

      
        sf::Vector2f direction = targetPosition - shape.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        moveDirection = direction / length; 
    }

    void update(float speed) {
      
        shape.move(moveDirection * speed);
    }

    bool isOffScreen(const sf::Vector2u& windowSize) const {
     
        return shape.getPosition().x < -shape.getRadius() * 2 ||
            shape.getPosition().x > windowSize.x + shape.getRadius() * 2 ||
            shape.getPosition().y < -shape.getRadius() * 2 ||
            shape.getPosition().y > windowSize.y + shape.getRadius() * 2;
    }

    sf::CircleShape getShape() const { return shape; }

private:
    sf::CircleShape shape;
    sf::Vector2f targetPosition;
    sf::Vector2f moveDirection;
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode(800, 600), "Balloon Game");

   
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        std::cerr << "Error loading background image!" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);


    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("music2.ogg")) {
        std::cerr << "Error loading music!" << std::endl;
        return -1;
    }
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    std::vector<Balloon> balloons;
    sf::Clock spawnClock;
    float balloonSpeed = 0.2f; 

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                   
                    for (size_t i = 0; i < balloons.size(); ++i) {
                        if (balloons[i].getShape().getGlobalBounds().contains(mousePos)) {
                            balloons.erase(balloons.begin() + i); 
                            break;
                        }
                    }
                }
            }
        }

        if (spawnClock.getElapsedTime().asSeconds() > 2.0f) {
            balloons.emplace_back(window.getSize());
            spawnClock.restart();
        }

        for (size_t i = 0; i < balloons.size();) {
            balloons[i].update(balloonSpeed);
            if (balloons[i].isOffScreen(window.getSize())) {
                balloons.erase(balloons.begin() + i);
            }
            else {
                ++i;
            }
        }

        window.clear();
        window.draw(backgroundSprite); 
        for (const auto& balloon : balloons) {
            window.draw(balloon.getShape());
        }
        window.display();
    }

    return 0;
}
