#include "universe.h"
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;

float G, epsilon, dx, dy, scale, k, friction;
int N;

void Universe::addEntity(const Entity& e) 
{
    entities.push_back(e);
}

void Universe::setConstants(float Gr, float EPSILON, int NEIGHBORS, float deltaX, float deltaY, float S, float K, float F) 
{
    ::G = Gr;
    ::epsilon = EPSILON;
    ::N = NEIGHBORS;
    ::dx = deltaX;
    ::dy = deltaY;
    ::scale = S;
    ::k = K;
    ::friction = F;
}

double Universe::computeHeight(int x, int y) 
{
    vector<pair<int, const Entity*>> distances;

    for (const auto& e : entities) 
    {
        int dx = x - e.posX;
        int dy = y - e.posY;
        int dist = sqrt(dx*dx + dy*dy);

        distances.push_back({dist, &e});
    }

    sort(distances.begin(), distances.end(),[](const auto& a, const auto& b){ return a.first < b.first; });

    float height = 0.0;

    for (int i = 0; i < min(N, (int)distances.size()); i++) 
    {
        const Entity* neighbor = distances[i].second;
        int dx = x - neighbor->posX;
        int dy = y - neighbor->posY;
        int mass = neighbor->mass;

        int r2 = dx*dx + dy*dy;

        height -= G * mass / (r2 + epsilon*epsilon);
    }

    return height;
}

void Universe::update() 
{
    for (auto& e : entities)
    {
        float h_x1 = computeHeight(e.posX + dx, e.posY);
        float h_x2 = computeHeight(e.posX - dx, e.posY);
        float h_y1 = computeHeight(e.posX, e.posY + dy);
        float h_y2 = computeHeight(e.posX, e.posY - dy);

        float slope_x = (h_x1 - h_x2) / (2 * dx);
        float slope_y = (h_y1 - h_y2) / (2 * dy);

        float ax = slope_x * scale;
        float ay = slope_y * scale;

        e.velX += ax;
        e.velY += ay;

        if(e.velX > k / sqrt(e.mass)) e.velX = k / sqrt(e.mass);
        if(e.velY > k / sqrt(e.mass)) e.velY = k / sqrt(e.mass);
        if(e.velX < -k / sqrt(e.mass)) e.velX = -k / sqrt(e.mass);
        if(e.velY < -k / sqrt(e.mass)) e.velY = -k / sqrt(e.mass);

        e.posX += e.velX;
        e.posY += e.velY;

        cout << "Entity " << e.name << " at (" << e.posX << ", " << e.posY << ") with velocity (" << e.velX << ", " << e.velY << ") with acc(" << ax << "," << ay << ")\n";
    }
}