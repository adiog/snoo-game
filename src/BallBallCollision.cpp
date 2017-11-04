#include "BallBallCollision.h"

#include "SnooCommons.h"

BallBallCollision::BallBallCollision(
        double t, BallPosition & first, BallPosition & second):
    Collision(t), firstBall(first), secondBall(second)
{
}

void BallBallCollision::apply(void)
{
    R3 dv = firstBall.v - secondBall.v;
    R3 normal = secondBall.pos - firstBall.pos;
    R3 dv2 = dv.projected(normal);

    firstBall.v -= dv2;
    secondBall.v += dv2;
}

ShotEvent BallBallCollision::getEvent(void)
{
    if (typeOfBall(firstBall.id) == WHITE_BALL)
        return ShotEvent(BALL_HIT_EVENT, typeOfBall(secondBall.id));
    if (typeOfBall(secondBall.id) == WHITE_BALL)
        return ShotEvent(BALL_HIT_EVENT, typeOfBall(firstBall.id));

    // jesli nic nie zwrocono...
    throw NoEventException();
}
