#include "GameStateEndOfGame.h"

#include "GameStateFactory.h"

#include <sstream>

GameStateEndOfGame::GameStateEndOfGame(Player p, SnookerModel & model):
    GameState(p, model)
{
}

GameStateEndOfGame::GameStateEndOfGame()
{
}

bool GameStateEndOfGame::isFinished(void)
{
    return true;
}

std::string GameStateEndOfGame::description(void)
{
    return "End of the game";
}

std::string GameStateEndOfGame::toString(void)
{
    std::ostringstream ss;
    ss << "E " << player.getId() << " " << wasWhiteBallPotted; 
    return ss.str();
}

GameState * GameStateEndOfGame::fromString(std::string s)
{
    GameStateEndOfGame * gs = new GameStateEndOfGame();
    s.erase(0, 1); // usuwamy pierwszy znak
    std::istringstream ss(s);
    int player;
    ss >> player >> gs->wasWhiteBallPotted;
    gs->player = player;
    return gs;
}
