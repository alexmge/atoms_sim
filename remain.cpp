#include "main.h"

#include <random>
#include <thread>

using namespace std;

#define NB_POINTS 1000
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define ACTION_RANGE 200

/**
 * @brief Creates an empty window
 */
sf::RenderWindow* createWindow()
{
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Render Window");
    return window;
}

/**
 * @brief creates a group of random points given the color
 */
void createPoints(point* points, sf::Color color)
{
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> disX(0, WINDOW_WIDTH);
    uniform_int_distribution<> disY(0, WINDOW_HEIGHT);

    for (int i = 0; i < NB_POINTS; i++)
    {
        points[i].position.x = disX(gen);
        points[i].position.y = disY(gen);
        points[i].color = color;
    }
}

/**
 * @brief draws the points to the window
 */
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

/**
 * @brief defines the relation between two groups of points, given the gravity constant
 * 
 * @param group1    the first group of points
 * @param group2    the second group of points
 * @param gravity   the gravity constant
 * 
 * @return sf::Vector2f
 */
sf::Vector2f computeForce(point* group1, point* group2, float gravity)
{
    sf::Vector2f force;
    for (int i = 0; i < NB_POINTS; i++)
    {
        force.x = 0;
        force.y = 0;
        for (int j = 0; j < NB_POINTS; j++)
        {
            sf::Vector2f distance = group1[i].position - group2[j].position;
            float distanceNorm = sqrt(distance.x * distance.x + distance.y * distance.y);

            if (distanceNorm > 0 && distanceNorm < ACTION_RANGE)
            {
                force.x += gravity * distance.x / (distanceNorm * distanceNorm);
                force.y += gravity * distance.y / (distanceNorm * distanceNorm);
            }
        }

        group1[i].velocity = group1[i].velocity + force * 0.1f;
        group1[i].position += group1[i].velocity;

        capVelocity(&group1[i], 2);
        boundPosition(&group1[i]);

    }
    return force;
}

/**
 * @brief main function
 */
int main()
{
    sf::RenderWindow* window = createWindow();
    window->setFramerateLimit(60);
    point* pointsBlue = new point[NB_POINTS];
    point* pointsRed = new point[NB_POINTS];
    point* pointsGreen = new point[NB_POINTS];

    createPoints(pointsBlue, sf::Color::Blue);
    createPoints(pointsRed, sf::Color::Red);
    createPoints(pointsGreen, sf::Color::Green);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        window->clear();
        drawPoints(window, pointsBlue);
        drawPoints(window, pointsRed);
        drawPoints(window, pointsGreen);

        // computeForce(pointsBlue, pointsGreen, -0.2);
        // computeForce(pointsRed, pointsGreen, -0.2);
        // computeForce(pointsGreen, pointsBlue, 0.2);
        // computeForce(pointsGreen, pointsRed, -0.2);
        // computeForce(pointsBlue, pointsBlue, 0.4);
        // computeForce(pointsRed, pointsRed, -0.2);
        // computeForce(pointsGreen, pointsGreen, -0.2);
        // computeForce(pointsBlue, pointsRed, 0);
        // computeForce(pointsRed, pointsBlue, 0);

        thread t1(computeForce, pointsBlue, pointsGreen, -0.2);
        thread t2(computeForce, pointsRed, pointsGreen, -0.2);
        thread t3(computeForce, pointsGreen, pointsBlue, -0.2);
        thread t4(computeForce, pointsGreen, pointsRed, 0.2);
        thread t5(computeForce, pointsBlue, pointsBlue, 0.3);
        thread t6(computeForce, pointsRed, pointsRed, 0);
        thread t7(computeForce, pointsGreen, pointsGreen, 0);
        thread t8(computeForce, pointsBlue, pointsRed, -0.7);
        thread t9(computeForce, pointsRed, pointsBlue, -0.2);

        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        t9.join();

        window->display();
    }

    return 0;
}