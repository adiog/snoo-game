#include "GameStatePotRed.h"

#include "GameStateFactory.h"
#include "GameStateChooseColor.h"
#include "GameStateFoul.h"
#include "RestoreBall.h"

#include <sstream>

GameStatePotRed::GameStatePotRed(
        Player p, SnookerModel & model, bool whitePotted):
    GameState(p, model, whitePotted)
{
}

GameStatePotRed::GameStatePotRed()
{
}

Decisions GameStatePotRed::getAvailableDecisions(SnookerModel & m)
{
    Decisions d;
    return d;
}

GameState * GameStatePotRed::applyShot(
        ShotEvents & shotEvents,
        SnookerModel & model, 
        std::string & desc)
{
    int points = 0;     // punkty za wbite bile
    int foul = 0;       // punkty za foul - dla przeciwnika
    bool miss = false;  // czy jesli jest faul - to jest miss?
    bool whitePotted = false; // czy biała została wbita?

    desc = "";

    // Sprawdzamy pierwszą trafiona bilę
    BallType first = shotEvents.firstBallHit();
    if (first != RED_BALL) {
        /// @TODO: nie zawsze będzie miss
        miss = true;
        foul = foulValue(first);
    }

    // Sprawdzamy co zostało wbite
    std::vector<ShotEvent> events = shotEvents.getEvents();
    for (unsigned i = 0; i < events.size(); i++) {
        const ShotEvent & event = events[i];
        if (event.type == BALL_IN_POCKET_EVENT) {
            desc = desc + ballName(event.ball);
            if (event.ball == RED_BALL)
                points++;
            else
                foul = newFaulValue(event.ball, foul);
        }
    }

    restoreColors(model);
    if (model.positions[0].potted) {
        whitePotted = true;
        RestoreBall::restoreWhiteBall(model);
    }

    if (foul == 0) {
        // bez foulu
        if (points > 0) {
            // wbito co najmniej jedną czerwoną bilę
            model.result.points[player] += points;
            model.result.currentBreak += points;
            return new GameStateChooseColor(player, model);
        } else {
            // nic nie wbito
            model.result.currentBreak = 0;
            return GameStateFactory::getFirstShotState(player.other(), model, whitePotted);
        }
    } else {
        // popełniono foul
        desc = "F" + desc;
        model.result.points[player.other()] += foul;
        model.result.currentBreak = 0;
        return new GameStateFoul(player.other(), model, whitePotted, miss);
    }
}
 
    
bool GameStatePotRed::canShot(void)
{
    return true;
}

std::string GameStatePotRed::description(void)
{
    return "Pot one of the red balls";
}

std::string GameStatePotRed::toString(void)
{
    std::ostringstream ss;
    ss << "R " << player.getId() << " " << wasWhiteBallPotted; 
    return ss.str();
}

GameState * GameStatePotRed::fromString(std::string s)
{
    GameStatePotRed * gs = new GameStatePotRed();
    s.erase(0, 1); // usuwamy pierwszy znak
    std::istringstream ss(s);
    int player;
    ss >> player >> gs->wasWhiteBallPotted;
    gs->player = player;
    return gs;
}
