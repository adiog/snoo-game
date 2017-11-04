#include "CushionDisk.h"

#include "SnooCommons.h"
#include "R3.h"
#include "BallPosition.h"

CushionDisk::CushionDisk(const R3 _s, double _r):
    s(_s), r(_r)
{
}

CushionDisk::~CushionDisk()
{
}

bool CushionDisk::collidesWith(const BallPosition & ball) const
{
    // Jeśli bila nie zbliża się do bandy, to kolizji nie ma
    if (!ball.pos.approaches(ball.v, s))
        return false;

    return (s.distanceTo(ball.pos) <= r + BALL_RADIUS);
}

double CushionDisk::distanceTo(const BallPosition & ball) const
{
    return fabs(ball.pos.distanceTo(s) - BALL_RADIUS - r);
}

R3 CushionDisk::calculateNormal(const BallPosition & ball) const
{
    return s - ball.pos;
}
