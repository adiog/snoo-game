#include "SnooCommons.h"

#include <cmath>
#include <sstream>

R3 initialRedPosition(const int i) {
    const double EPSILON = 0.05;
    const R3 row(BALL_RADIUS + EPSILON, (BALL_RADIUS + EPSILON) * sqrt(3.0));
    const R3 place(-2.0 * (BALL_RADIUS + EPSILON), 0);
    const R3 top = PINK_SPOT + R3(0, 2.0 * (BALL_RADIUS + EPSILON));

    if (i == 0)
        return top;
    else if (i < 3)
        return top + row + place * (i - 1);
    else if (i < 6)
        return top + row * 2 + place * (i - 3);
    else if (i < 10)
        return top + row * 3 + place * (i - 6);
    else if (i < 15)
        return top + row * 4 + place * (i - 10);
    else
        throw OutOfRangeException();
}

int ballValue(BallType ball)
{
    switch (ball) {
        case WHITE_BALL:  return 0;
        case RED_BALL:    return 1;
        case YELLOW_BALL: return 2;
        case GREEN_BALL:  return 3;
        case BROWN_BALL:  return 4;
        case BLUE_BALL:   return 5;
        case PINK_BALL:   return 6;
        case BLACK_BALL:  return 7;
        case NO_BALL:     return 0;
        default:          return 0;
    }
}

std::string ballName(BallType ball)
{
    switch (ball) {
        case WHITE_BALL:  return "0";
        case RED_BALL:    return "1";
        case YELLOW_BALL: return "2";
        case GREEN_BALL:  return "3";
        case BROWN_BALL:  return "4";
        case BLUE_BALL:   return "5";
        case PINK_BALL:   return "6";
        case BLACK_BALL:  return "7";
        case NO_BALL:     return "";
        default:          return "";
    }
}

std::string intToString(int i)
{
    std::ostringstream ss;
    ss << i;
    return ss.str();
}
