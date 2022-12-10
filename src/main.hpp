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
};


// Quadtree class that will be used with the Barnes-Hut algorithm.
// It is a recursive structure that will be used to store the points.
// Each node must store the center of mass of the points it contains, the number of points of each color it contains and the center of mass of each color.
// It must also store the total number of points it contains.
class Quadtree
{
public:
    Quadtree(sf::Vector2f center, sf::Vector2f bounds, int depth);
    ~Quadtree();

    void insert(point* p);
    void update();
    void draw(sf::RenderWindow* window);
    void clear();
    int contains(point* p);

    sf::Vector2f center;
    sf::Vector2f bounds;
    int depth;
    int nb_points;
    int nb_blue;
    int nb_red;
    int nb_green;
    sf::Vector2f center_of_mass;
    sf::Vector2f center_of_mass_blue;
    sf::Vector2f center_of_mass_red;
    sf::Vector2f center_of_mass_green;
    Quadtree* children[4];
    std::vector<point*> points;
};

extern float blue_to_red;
extern float red_to_blue;
extern float blue_to_blue;
extern float red_to_red;
extern float blue_to_green;
extern float red_to_green;
extern float green_to_blue;
extern float green_to_red;
extern float green_to_green;

static inline float getGravity(sf::Color color1, sf::Color color2)
{
    if (color1 == sf::Color::Blue && color2 == sf::Color::Red)
    {
        return blue_to_red;
    }
    if (color1 == sf::Color::Red && color2 == sf::Color::Blue)
    {
        return red_to_blue;
    }
    if (color1 == sf::Color::Blue && color2 == sf::Color::Blue)
    {
        return blue_to_blue;
    }
    if (color1 == sf::Color::Red && color2 == sf::Color::Red)
    {
        return red_to_red;
    }
    if (color1 == sf::Color::Blue && color2 == sf::Color::Green)
    {
        return blue_to_green;
    }
    if (color1 == sf::Color::Red && color2 == sf::Color::Green)
    {
        return red_to_green;
    }
    if (color1 == sf::Color::Green && color2 == sf::Color::Blue)
    {
        return green_to_blue;
    }
    if (color1 == sf::Color::Green && color2 == sf::Color::Red)
    {
        return green_to_red;
    }
    if (color1 == sf::Color::Green && color2 == sf::Color::Green)
    {
        return green_to_green;
    }
    return 0;
}