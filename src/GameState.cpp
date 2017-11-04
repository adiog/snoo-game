#include "GameState.h"

#include <vector>
#include "SnookerModel.h"
#include "Decision.h"
#include "ShotEvents.h"
#include "RestoreBall.h"

GameState::GameState():
    player(0),
    wasWhiteBallPotted(false)
{
}

GameState::GameState(Player p, SnookerModel & model, bool whitePotted):
    player(p), wasWhiteBallPotted(whitePotted)
{
}

GameState::~GameState()
{
}

std::vector<Decision> GameState::getAvailableDecisions(SnookerModel &)
{
    return std::vector<Decision>();
}

GameState * GameState::applyDecision(Decision &, SnookerModel &, std::string &)
{
    throw NotApplicableException();
}

GameState * GameState::applyShot(ShotEvents &, SnookerModel &, std::string &)
{
    throw NotApplicableException();
}

bool GameState::canMoveWhiteBall(void)
{
    return wasWhiteBallPotted;
}

bool GameState::canShot(void)
{
    return false;
}

bool GameState::isFinished(void)
{
    return false;
}

std::string GameState::description(void)
{
    return "Erroneus state - restart the game :)";
}

std::string GameState::toString(void)
{
    return "G";
}
        
GameState * GameState::fromString(std::string)
{
    return new GameState();
}

Player GameState::getPlayer(void)
{
    return player;
}

void GameState::setPlayer(Player p)
{
    player = p;
}

int GameState::foulValue(BallType ball)
{
    int value = ballValue(ball);
    if (value < 4)
        value = 4;
    return value;
}

int GameState::newFaulValue(BallType ball, int foul)
{
    if (foulValue(ball) > foul)
        return foulValue(ball);
    else
        return foul;
}

void GameState::restoreColors(SnookerModel & model)
{
    if (model.positions[BLACK_BALL].potted)
        RestoreBall::restoreBall(model, BLACK_BALL, BLACK_SPOT);
    
    if (model.positions[PINK_BALL].potted)
        RestoreBall::restoreBall(model, PINK_BALL, PINK_SPOT);
    
    if (model.positions[BLUE_BALL].potted)
        RestoreBall::restoreBall(model, BLUE_BALL, BLUE_SPOT);
    
    if (model.positions[BROWN_BALL].potted)
        RestoreBall::restoreBall(model, BROWN_BALL, BROWN_SPOT);
    
    if (model.positions[GREEN_BALL].potted)
        RestoreBall::restoreBall(model, GREEN_BALL, GREEN_SPOT);
    
    if (model.positions[YELLOW_BALL].potted)
        RestoreBall::restoreBall(model, YELLOW_BALL, YELLOW_SPOT); 
}
