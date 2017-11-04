#include <cmath>
#include <cstdlib>
#include <iostream>

#include "SnookerModel.h"
#include "BallPositions.h"
#include "SnooCommons.h"
#include "GameStatePotRed.h"
#include "Encoder.h"

SnookerModel::SnookerModel() {
    time = lastTime = 0.0;
    vectorDirection = PI / 2.0;
    vectorSize = 0.0;
    tilt = 0.5;
    shotInProgress = false; 
    gameState = new GameStatePotRed(Player(0), *this);
}

SnookerModel::~SnookerModel() {
}

R3 SnookerModel::getDirectionAsVector(void)
{
    return R3(cos(vectorDirection), sin(vectorDirection)).unit();
}

bool SnookerModel::anyBallMoving(void)
{
    for (int i = 0; i < MAX_BALLS; i++)
        if (positions[i].v.length() > 0)
            return true;
    return false;
}

void SnookerModel::initBallVelocity(void)
{
    for (int i = 0; i < MAX_BALLS; i++) {
        positions[i].v = R3::ZERO;
        positions[i].w = R3::ZERO;
    }
} 


void SnookerModel::initBallPositions(void)
{
    srand(0);
    initBallVelocity();
    positions[0].pos = BROWN_SPOT + R3(10, -10);
    positions[1].pos = YELLOW_SPOT;
    positions[2].pos = GREEN_SPOT;
    positions[3].pos = BROWN_SPOT;
    positions[4].pos = BLUE_SPOT;
    positions[5].pos = PINK_SPOT;
    positions[6].pos = BLACK_SPOT;
    for (int i = 0; i < 15; i++) {
        positions[i+7].pos = initialRedPosition(i);
        // lekko zaburzamy pozycje bil
        positions[i+7].pos[0] += (rand() % 1000 - 500) / 5000.0;
        positions[i+7].pos[0] += (rand() % 1000 - 500) / 5000.0;
    }
    for (int i = 0; i < MAX_BALLS; i++)
        positions[i].potted = false;
    begin = positions;
}

void SnookerModel::startNewGame(void)
{
    initBallPositions();
    shotInProgress = false;
    result.points[0] = result.points[1] = 0;
    result.frames[0] = result.frames[1] = 0;
    result.currentBreak = 0;
    if (gameState != NULL)
        delete gameState;
    gameState = new GameStatePotRed(Player(0), *this, true);
}

void SnookerModel::setPositionsFromString(const std::string & s)
{
    positions.setFromString(s);
}

std::string SnookerModel::shotDataToString()
{
    double data[5];
    data[0] = vectorDirection;
    data[1] = vectorSize;
    data[2] = tilt;
    data[3] = whiteBallPoint[0];
    data[4] = whiteBallPoint[1];
    return Encoder::encode(data, sizeof(data));
}

void SnookerModel::setShotDataFromString(const std::string & s)
{
    double data[5];
    Encoder::decode(s, data);
    vectorDirection = data[0];
    vectorSize = data[1];
    tilt = data[2];
    whiteBallPoint[0] = data[3];
    whiteBallPoint[1] = data[4];
}
