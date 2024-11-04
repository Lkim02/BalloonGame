#include "Balloon.h"
#include <cstdlib>

Balloon::Balloon(float radius, const sf::Vector2u& windowSize) {
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Red);

 
    float xPos = static_cast<float>(std::rand() % (windowSize.x - static_cast<unsigned int>(2 * radius)));
    shape.setPosition(xPos, static_cast<float>(windowSize.y));


    speed = 5.0f + static_cast<float>(std::rand() % 10);  
}

const sf::CircleShape& Balloon::getShape() const {
    return shape;
}

void Balloon::update() {
   
    shape.move(0, -speed * 0.005f); 
}

bool Balloon::isOffScreen() const {
    return shape.getPosition().y + shape.getRadius() * 2 < 0;
}
