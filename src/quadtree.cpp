#include "main.hpp"

float blue_to_red = -0.1f;
float red_to_blue = 0.3f;
float blue_to_blue = -0.2f;
float red_to_red = 0.1f;
float blue_to_green = -0.5f;
float red_to_green = -0.4f;
float green_to_blue = -0.1f;
float green_to_red = -0.4f;
float green_to_green = -0.3f;

Quadtree::Quadtree(sf::Vector2f center, sf::Vector2f bounds, int depth)
{
    this->center = center;
    this->bounds = bounds;
    this->depth = depth;
    this->nb_points = 0;
    this->nb_blue = 0;
    this->nb_red = 0;
    this->nb_green = 0;
    this->center_of_mass = sf::Vector2f(0, 0);
    this->center_of_mass_blue = sf::Vector2f(0, 0);
    this->center_of_mass_red = sf::Vector2f(0, 0);
    this->center_of_mass_green = sf::Vector2f(0, 0);
    for (int i = 0; i < 4; i++)
    {
        this->children[i] = nullptr;
    }
}

Quadtree::~Quadtree()
{
    for (int i = 0; i < 4; i++)
    {
        if (this->children[i] != nullptr)
        {
            delete this->children[i];
        }
    }
}

void Quadtree::clear()
{
    this->nb_points = 0;
    this->nb_blue = 0;
    this->nb_red = 0;
    this->nb_green = 0;
    this->center_of_mass = sf::Vector2f(0, 0);
    this->center_of_mass_blue = sf::Vector2f(0, 0);
    this->center_of_mass_red = sf::Vector2f(0, 0);
    this->center_of_mass_green = sf::Vector2f(0, 0);
    for (int i = 0; i < 4; i++)
    {
        if (this->children[i] != nullptr)
        {
            this->children[i]->clear();
        }
    }
}

void Quadtree::update()
{
    this->nb_points = 0;
    this->nb_blue = 0;
    this->nb_red = 0;
    this->nb_green = 0;
    this->center_of_mass = sf::Vector2f(0, 0);
    this->center_of_mass_blue = sf::Vector2f(0, 0);
    this->center_of_mass_red = sf::Vector2f(0, 0);
    this->center_of_mass_green = sf::Vector2f(0, 0);
    for (int i = 0; i < 4; i++)
    {
        if (this->children[i] != nullptr)
        {
            this->children[i]->update();
            this->nb_points += this->children[i]->nb_points;
            this->nb_blue += this->children[i]->nb_blue;
            this->nb_red += this->children[i]->nb_red;
            this->nb_green += this->children[i]->nb_green;
            this->center_of_mass += this->children[i]->center_of_mass;
            this->center_of_mass_blue += this->children[i]->center_of_mass_blue;
            this->center_of_mass_red += this->children[i]->center_of_mass_red;
            this->center_of_mass_green += this->children[i]->center_of_mass_green;
        }
    }
    for (int i = 0; i < this->points.size(); i++)
    {
        this->nb_points++;
        this->center_of_mass += this->points[i]->position;
        if (this->points[i]->color == sf::Color::Blue)
        {
            this->nb_blue++;
            this->center_of_mass_blue += this->points[i]->position;
        }
        else if (this->points[i]->color == sf::Color::Red)
        {
            this->nb_red++;
            this->center_of_mass_red += this->points[i]->position;
        }
        else if (this->points[i]->color == sf::Color::Green)
        {
            this->nb_green++;
            this->center_of_mass_green += this->points[i]->position;
        }
    }
    if (this->nb_points > 0)
    {
        this->center_of_mass = this->center_of_mass / (float)this->nb_points;
    }
    if (this->nb_blue > 0)
    {
        this->center_of_mass_blue = this->center_of_mass_blue / (float)this->nb_blue;
    }
    if (this->nb_red > 0)
    {
        this->center_of_mass_red = this->center_of_mass_red / (float)this->nb_red;
    }
    if (this->nb_green > 0)
    {
        this->center_of_mass_green = this->center_of_mass_green / (float)this->nb_green;
    }
}

void Quadtree::insert(point* p)
{
    // case leaf already contains a point
    if (this->nb_points == 1)
    {
        // create children
        for (int i = 0; i < 4; i++)
        {
            sf::Vector2f new_center = this->center;
            sf::Vector2f new_bounds = this->bounds / 2.0f;
            if (i == 0)
            {
                new_center.x -= new_bounds.x / 2.0f;
                new_center.y -= new_bounds.y / 2.0f;
            }
            else if (i == 1)
            {
                new_center.x += new_bounds.x / 2.0f;
                new_center.y -= new_bounds.y / 2.0f;
            }
            else if (i == 2)
            {
                new_center.x -= new_bounds.x / 2.0f;
                new_center.y += new_bounds.y / 2.0f;
            }
            else if (i == 3)
            {
                new_center.x += new_bounds.x / 2.0f;
                new_center.y += new_bounds.y / 2.0f;
            }
            this->children[i] = new Quadtree(new_center, new_bounds, this->depth + 1);
        }
        // insert point in children
        for (int i = 0; i < 4; i++)
        {
            if (this->children[i]->contains(this->points[0]))
            {
                this->children[i]->insert(this->points[0]);
            }
        }
        this->points.clear();
        this->nb_points = 0;
    }
    // case leaf is empty
    if (this->nb_points == 0)
    {
        this->points.push_back(p);
        this->nb_points++;
        this->center_of_mass = p->position;
        if (p->color == sf::Color::Blue)
        {
            this->nb_blue++;
            this->center_of_mass_blue = p->position;
        }
        else if (p->color == sf::Color::Red)
        {
            this->nb_red++;
            this->center_of_mass_red = p->position;
        }
        else if (p->color == sf::Color::Green)
        {
            this->nb_green++;
            this->center_of_mass_green = p->position;
        }
    }
    // case leaf contains more than one point
    else
    {
        this->points.push_back(p);
        this->nb_points++;
        this->center_of_mass += p->position;
        if (p->color == sf::Color::Blue)
        {
            this->nb_blue++;
            this->center_of_mass_blue += p->position;
        }
        else if (p->color == sf::Color::Red)
        {
            this->nb_red++;
            this->center_of_mass_red += p->position;
        }
        else if (p->color == sf::Color::Green)
        {
            this->nb_green++;
            this->center_of_mass_green += p->position;
        }
        // insert point in children
        for (int i = 0; i < 4; i++)
        {
            if (this->children[i]->contains(p))
            {
                this->children[i]->insert(p);
            }
        }
    }

    this->update();
}

void Quadtree::draw(sf::RenderWindow* window)
{
    sf::RectangleShape rect;
    rect.setPosition(this->center.x - this->bounds.x / 2, this->center.y - this->bounds.y / 2);
    rect.setSize(this->bounds);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(1);
    window->draw(rect);
    for (int i = 0; i < 4; i++)
    {
        if (this->children[i] != nullptr)
        {
            this->children[i]->draw(window);
        }
    }
}