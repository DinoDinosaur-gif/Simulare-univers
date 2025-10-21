#include "entity.h"

Entity::Entity(string t, string n, string d,
               int m, int den, int vol,
               float x, float y, int rot,
               float vx, float vy)
    : type(t), name(n), description(d),
      mass(m), density(den), volume(vol),
      posX(x), posY(y), rotation(rot),
      velX(vx), velY(vy) {}

void Entity::setInfo(string t, string n, string d,
                     int m, int den, int vol,
                     float x, float y, int rot,
                     float vx, float vy)
{
    type = t; name = n; description = d;
    mass = m; density = den; volume = vol;
    posX = x; posY = y; rotation = rot;
    velX = vx; velY = vy;
}