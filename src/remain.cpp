#include "main.hpp"

#include <random>
#include <thread>

#define NB_POINTS 100
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define ACTION_RANGE 100
#define SOFTENING 100

sf::RenderWindow* createWindow()
{
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Render Window");
    return window;
}

void createPoints(point* points)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, WINDOW_WIDTH);
    std::uniform_int_distribution<> dis2(0, WINDOW_HEIGHT);
    std::uniform_int_distribution<> dis3(0, 2);

    for (int i = 0; i < NB_POINTS; i++)
    {
        points[i].position.x = dis(gen);
        points[i].position.y = dis2(gen);
        points[i].velocity.x = 0;
        points[i].velocity.y = 0;
        switch (dis3(gen))
        {
        case 0:
            points[i].color = sf::Color::Blue;
            break;
        case 1:
            points[i].color = sf::Color::Red;
            break;
        case 2:
            points[i].color = sf::Color::Green;
            break;
        }
    }
}

void drawPoints(sf::RenderWindow* window, point* points)
{
    for (int i = 0; i < NB_POINTS; i++)
    {
        sf::CircleShape circle(2);
        circle.setPosition(points[i].position);
        circle.setFillColor(points[i].color);
        window->draw(circle);
    }
}

/**
 * @brief Cap the velocity of a given point to a given value
 */
void capVelocity(point* point, float maxVelocity)
{
    if (point->velocity.x > maxVelocity)
    {
        point->velocity.x = maxVelocity;
    }
    if (point->velocity.x < -maxVelocity)
    {
        point->velocity.x = -maxVelocity;
    }
    if (point->velocity.y > maxVelocity)
    {
        point->velocity.y = maxVelocity;
    }
    if (point->velocity.y < -maxVelocity)
    {
        point->velocity.y = -maxVelocity;
    }
}

/**
 * @brief bound the position of a given point to the window and reverse its velocity
 */
void boundPosition(point* point)
{
    if (point->position.x > WINDOW_WIDTH)
    {
        point->position.x = WINDOW_WIDTH;
        point->velocity.x = -point->velocity.x;
    }
    if (point->position.x < 0)
    {
        point->position.x = 0;
        point->velocity.x = -point->velocity.x;
    }
    if (point->position.y > WINDOW_HEIGHT)
    {
        point->position.y = WINDOW_HEIGHT;
        point->velocity.y = -point->velocity.y;
    }
    if (point->position.y < 0)
    {
        point->position.y = 0;
        point->velocity.y = -point->velocity.y;
    }
}

/* void force_thread(point *group, int i, sf::Vector2f force)
{
    for (int j = 0; j != i && j < NB_POINTS; j++)
    {
        sf::Vector2f distance = group[i].position - group[j].position;
        float distanceNorm = sqrt(distance.x * distance.x + distance.y * distance.y);

        if (distanceNorm > 0 && distanceNorm < ACTION_RANGE)
        {
            force.x += getGravity(group[i].color, group[j].color) * distance.x / (distanceNorm * distanceNorm + SOFTENING);
            force.y += getGravity(group[i].color, group[j].color) * distance.y / (distanceNorm * distanceNorm + SOFTENING);
        }
    }
    group[i].velocity = group[i].velocity + force * 0.5f;
    group[i].position += group[i].velocity;

    boundPosition(&group[i]);
    capVelocity(&group[i], 2);
}

void computeForceAll(point* group)
{
    std::vector<std::thread> threads;
    for (int i = 0; i < NB_POINTS; i++)
    {
        threads.push_back(std::thread(force_thread, group, i, sf::Vector2f(0, 0)));
    }
    for (int i = 0; i < NB_POINTS; i++)
    {
        threads[i].join();
    }
} */

/**
 * @brief compute the force between all the points using the quadtree
 */
void computeForceAll(point* group, sf::RenderWindow* window, Quadtree* tree)
{
    
}

/**
 * @brief main function
 */
int main()
{
    sf::RenderWindow* window = createWindow();
    window->setFramerateLimit(30);

    point* points = new point[NB_POINTS];
    createPoints(points);

    Quadtree* tree = new Quadtree(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), 0);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        window->clear();
        tree->clear();

        for (int i = 0; i < NB_POINTS; i++)
        {
            tree->insert(&points[i]);
        }
        
        drawPoints(window, points);
        tree->draw(window);

        window->display();
    }

    return 0;
}
