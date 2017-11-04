#include "ShotEvents.h"

#include <vector>

void ShotEvents::addEvent(ShotEvent e)
{
    events.push_back(e);
}

void ShotEvents::removeAll(void)
{
    events.clear();
}

std::vector<ShotEvent> & ShotEvents::getEvents(void)
{
    return events;
}

int ShotEvents::count(void)
{
    return events.size();
}

BallType ShotEvents::firstBallHit(void)
{
    for (unsigned int i = 0; i < events.size(); i++)
        if (events[i].type == BALL_HIT_EVENT)
            return events[i].ball;
    return NO_BALL;
}
