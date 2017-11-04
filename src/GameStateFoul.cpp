#include "GameStateFoul.h"

#include "GameStateFactory.h"

#include <sstream>

GameStateFoul::GameStateFoul(Player p, SnookerModel & model, bool whitePotted, bool wasMiss):
    GameState(p, model, whitePotted), miss(wasMiss)
{
}

GameStateFoul::GameStateFoul()
{
}

Decisions GameStateFoul::getAvailableDecisions(SnookerModel & m)
{
    Decisions d;
    d.push_back(Decision(DECISION_CONTINUE));
    d.push_back(Decision(DECISION_FORCE_OPPONENT));
    if (miss)
        d.push_back(Decision(DECISION_MISS));

    return d;
}

GameState * GameStateFoul::applyDecision(
        Decision & decision,
        SnookerModel & model,
        std::string &)
{
    switch (decision.getId()) {
        case DECISION_FORCE_OPPONENT:
            return GameStateFactory::getFirstShotState(player.other(), model, wasWhiteBallPotted);
        case DECISION_CONTINUE:
            return GameStateFactory::getFirstShotState(player, model, wasWhiteBallPotted);
        case DECISION_MISS:
            model.positions = model.begin;
            return GameStateFactory::deserialize(model.beginGameState);
        default:
            throw NotApplicableException();
    }
}

std::string GameStateFoul::description(void)
{
    if (miss)
        return "Foul and a miss!";
    else
        return "Foul!";
}
     
std::string GameStateFoul::toString(void)
{
    std::ostringstream ss;
    ss << "F " << player.getId() << " " << wasWhiteBallPotted << " " << miss; 
    return ss.str();
}

GameState * GameStateFoul::fromString(std::string s)
{
    GameStateFoul * gs = new GameStateFoul();
    s.erase(0, 1); // usuwamy pierwszy znak
    std::istringstream ss(s);
    int player;
    ss >> player >> gs->wasWhiteBallPotted >> gs->miss;
    gs->player = player;
    return gs;
}
