#include "BallPosition.h"
#include "R3.h"

BallType typeOfBall(int id)
{
    switch (id) {
        case 0: return WHITE_BALL;
        case 1: return YELLOW_BALL;
        case 2: return GREEN_BALL;
        case 3: return BROWN_BALL;
        case 4: return BLUE_BALL;
        case 5: return PINK_BALL;
        case 6: return BLACK_BALL;
        default: return RED_BALL;
    }
}

BallPosition::BallPosition() {
    potted = false;
}

BallPosition::BallPosition(R3 &initial):
    pos(initial)
{ 
    potted = false;
}

BallPosition::~BallPosition() {
}

