#include "GameStateTraining.h"

#include "RestoreBall.h"

#include <sstream>

GameStateTraining::GameStateTraining(
        Player p, SnookerModel & model):
    GameState(p, model, true)
{
}

GameStateTraining::GameStateTraining()
{
}

Decisions GameStateTraining::getAvailableDecisions(SnookerModel & m)
{
    Decisions d;
    return d;
}

GameState * GameStateTraining::applyShot(
        ShotEvents & shotEvents,
        SnookerModel & model,
        std::string &)
{
    restoreColors(model);
    if (model.positions[0].potted)
        RestoreBall::restoreWhiteBall(model);

    return new GameStateTraining(player, model);
}
 
    
bool GameStateTraining::canShot(void)
{
    return true;
}

std::string GameStateTraining::description(void)
{
    return "Training, pot any ball.";
}

std::string GameStateTraining::toString(void)
{
    std::ostringstream ss;
    ss << "T ";
    return ss.str();
}

GameState * GameStateTraining::fromString(std::string s)
{
    GameStateTraining * gs = new GameStateTraining();
    return gs;
}
