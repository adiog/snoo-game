#include "GameStateCleanColor.h"

#include "GameStateFactory.h"
#include "GameStateFoul.h"
#include "GameStateEndOfGame.h"
#include "RestoreBall.h"

#include <sstream>

GameStateCleanColor::GameStateCleanColor(
        Player p,
        SnookerModel & model,
        int pointsForBall,
        bool whitePotted
        ):
    GameState(p, model, whitePotted), whichColor(pointsForBall)
{
}

GameStateCleanColor::GameStateCleanColor()
{
}

Decisions GameStateCleanColor::getAvailableDecisions(SnookerModel & m)
{
    Decisions d;
    return d;
}

GameState * GameStateCleanColor::applyShot(
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
    if (ballValue(first) != whichColor) {
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
            if (ballValue(event.ball) == whichColor)
                points += whichColor;
            else
                foul = newFaulValue(event.ball, foul);
        }
    }

    if (foul > 0 && foul < whichColor)
        foul = whichColor;

    if (model.positions[0].potted) {
        whitePotted = true;
        RestoreBall::restoreWhiteBall(model);
    }

    if (foul == 0) {
        // bez foulu
        if (points > 0) {
            // wbito deklarowaną
            model.result.points[player] += points;
            model.result.currentBreak += points;
            return GameStateFactory::getFirstShotState(player, model);
        } else {
            // nic nie wbito
            model.result.currentBreak = 0;
            return GameStateFactory::getFirstShotState(player.other(), model);
        }
    } else {
        // popełniono foul
        desc = "F" + desc;
        model.result.points[player.other()] += foul;
        model.result.currentBreak = 0;
        if (whichColor == 7) {
            model.positions[6].potted = true;
            return GameStateFactory::getFirstShotState(player.other(), model);
        }
        return new GameStateFoul(player.other(), model, whitePotted, miss); 
    }
}
 
std::string GameStateCleanColor::description(void)
{
    std::string color;
    switch (whichColor) {
        case 2: color = "yellow"; break;
        case 3: color = "green"; break;
        case 4: color = "brown"; break;
        case 5: color = "blue"; break;
        case 6: color = "pink"; break;
        case 7: color = "black"; break;
        default: color = "? ? ! !"; break;
    }
    return "Pot the " + color + " ball";
}
    
bool GameStateCleanColor::canShot(void)
{
    return true;
}

std::string GameStateCleanColor::toString(void)
{
    std::ostringstream ss;
    ss << "V " << player.getId() << " " << whichColor;
    return ss.str();
}

GameState * GameStateCleanColor::fromString(std::string s)
{
    GameStateCleanColor * gs = new GameStateCleanColor();
    s.erase(0, 1); // usuwamy pierwszy znak
    std::istringstream ss(s);
    int player, ball;
    ss >> player >> ball;
    gs->player = player;
    gs->wasWhiteBallPotted = false;
    gs->whichColor = ball;
    return gs;
}
