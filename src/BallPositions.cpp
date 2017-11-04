#include "BallPositions.h"

#include <sstream>
#include "BallPosition.h"
#include "SnooCommons.h"
#include "Encoder.h"

BallPositions::BallPositions()
{
    for (int i = 0; i < MAX_BALLS; i++)
        positions[i].id = i;
}

BallPosition & BallPositions::operator[](const int i) {
#ifndef SNOO_UNSAFE
    if (i >= MAX_BALLS) {
        throw OutOfRangeException();
    }
#endif
    return positions[i];
}

std::string BallPositions::toString(void) {
    double tab[2 * MAX_BALLS];

    for (int i = 0; i < MAX_BALLS; i++) {
        BallPosition & ball = positions[i];
        double * data = tab + i * 2;
        if (ball.potted) {
            data[0] = 10000 + ball.pos[0];
            data[1] = 10000 + ball.pos[1];
        } else {
          data[0] = ball.pos[0];
          data[1] = ball.pos[1];
        }
    }

    return Encoder::encode(tab, sizeof(tab));
}

void BallPositions::setFromString(const std::string & s) {
    double tab[2 * MAX_BALLS];
    Encoder::decode(s, tab);

    for (int i = 0; i < MAX_BALLS; i++) {
        BallPosition & ball = positions[i];
        double * data = tab + i * 2;
        if (data[0] > 5000) {
            ball.potted = true;
            ball.pos[0] = data[0] - 10000;
            ball.pos[1] = data[1] - 10000;
        } else {
            ball.potted = false;
            ball.pos[0] = data[0];
            ball.pos[1] = data[1];
        }
        ball.pos[2] = 0.0;
        ball.v = R3::ZERO;
        ball.w = R3::ZERO;
    }
}
