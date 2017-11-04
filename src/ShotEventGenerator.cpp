#include "ShotEventGenerator.h"

ShotEventGenerator::~ShotEventGenerator()
{
}

void ShotEventGenerator::addListener(ShotEventListener * listener)
{
    listeners.push_back(listener);
}

void ShotEventGenerator::generateEvent(const ShotEvent & event)
{
    for (unsigned int i = 0; i < listeners.size(); i++)
        listeners[i]->handleShotEvent(event);
}
