#pragma once

#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

struct point {
    sf::Color color;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
};