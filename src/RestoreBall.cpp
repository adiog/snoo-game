#include "RestoreBall.h"

#include <cstdlib>
#include "SnooCommons.h"

// true <=> ball placed in place doesn't collide with any other ball
bool RestoreBall::canPlaceBall(SnookerModel & model, R3 place)
{
    for (int i = 0; i < MAX_BALLS; i++)
        if (model.positions[i].pos.distanceTo(place) < 2 * BALL_RADIUS
            && (! model.positions[i].potted))
            return false;
    return true;
}

void RestoreBall::restoreBall(SnookerModel & model, int ballId, R3 place)
{
    R3 where = place;
    if (canPlaceBall(model, place))
        where = place;
    else if (canPlaceBall(model, BLACK_SPOT))
        where = BLACK_SPOT;
    else if (canPlaceBall(model, PINK_SPOT))
        where = PINK_SPOT;
    else if (canPlaceBall(model, BLUE_SPOT))
        where = BLUE_SPOT;
    else if (canPlaceBall(model, BROWN_SPOT))
        where = BROWN_SPOT;
    else if (canPlaceBall(model, GREEN_SPOT))
        where = GREEN_SPOT;
    else if (canPlaceBall(model, YELLOW_SPOT))
        where = YELLOW_SPOT;
    else while (!canPlaceBall(model, where))
        where += R3(0, 0.01);

    model.positions[ballId].potted = false;
    model.positions[ballId].pos = where;
}

void RestoreBall::restoreWhiteBall(SnookerModel & model)
{
    R3 where;

    do {
        where = BROWN_SPOT;
        where[0] += (rand() * 1.0 / RAND_MAX - 0.5) * 2.0 * D_RADIUS;
        where[1] += (rand() * 1.0 / RAND_MAX) * (-D_RADIUS);
    } while (!(where.distanceTo(BROWN_SPOT) < D_RADIUS && canPlaceBall(model, where)));

    model.positions[0].potted = false;
    model.positions[0].pos = where;
}
