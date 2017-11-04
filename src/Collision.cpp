#include "Collision.h"

Collision::Collision(double t):
    time(t)
{
}

Collision::~Collision()
{
}

bool Collision::isEvent(void)
{
    try {
        getEvent();
        return true;
    } catch (NoEventException e) {
        return false;
    }
}

ShotEvent Collision::getEvent(void)
{
    throw NoEventException();
}
