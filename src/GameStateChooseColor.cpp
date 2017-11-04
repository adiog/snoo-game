#include "GameStateChooseColor.h"

#include "GameStatePotRed.h"
#include "GameStatePotColor.h"

#include <sstream>

GameStateChooseColor::GameStateChooseColor(Player p, SnookerModel & model):
    GameState(p, model)
{
}

GameStateChooseColor::GameStateChooseColor()
{
}

Decisions GameStateChooseColor::getAvailableDecisions(SnookerModel & m)
{
    Decisions d;
    d.push_back(Decision(DECISION_YELLOW));
    d.push_back(Decision(DECISION_GREEN));
    d.push_back(Decision(DECISION_BROWN));
    d.push_back(Decision(DECISION_BLUE));
    d.push_back(Decision(DECISION_PINK));
    d.push_back(Decision(DECISION_BLACK));
    
    return d;
}

GameState * GameStateChooseColor::applyDecision(
        Decision & decision,
        SnookerModel & model,
        std::string &)
{
    switch (decision.getId()) {
        case DECISION_YELLOW:
            return new GameStatePotColor(player, model, ballValue(YELLOW_BALL));
        case DECISION_GREEN:
            return new GameStatePotColor(player, model, ballValue(GREEN_BALL));
        case DECISION_BROWN:
            return new GameStatePotColor(player, model, ballValue(BROWN_BALL));
        case DECISION_BLUE:
            return new GameStatePotColor(player, model, ballValue(BLUE_BALL));
        case DECISION_PINK:
            return new GameStatePotColor(player, model, ballValue(PINK_BALL));
        case DECISION_BLACK:
            return new GameStatePotColor(player, model, ballValue(BLACK_BALL));
        default:
            throw NotApplicableException();
    }
}

std::string GameStateChooseColor::description(void)
{
    return "Choose ball to pot";
}
     
std::string GameStateChooseColor::toString(void)
{
    std::ostringstream ss;
    ss << "C " << player.getId() << " " << wasWhiteBallPotted; 
    return ss.str();
}

GameState * GameStateChooseColor::fromString(std::string s)
{
    GameStateChooseColor * gs = new GameStateChooseColor();
    s.erase(0, 1); // usuwamy pierwszy znak
    std::istringstream ss(s);
    int player;
    ss >> player >> gs->wasWhiteBallPotted;
    gs->player = player;
    return gs;
}
