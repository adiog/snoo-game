#include "GameStateFactory.h"

#include "SnookerModel.h"
#include "SnooCommons.h"
#include "GameStatePotRed.h"
#include "GameStateChooseColor.h"
#include "GameStatePotColor.h"
#include "GameStateCleanColor.h"
#include "GameStateFoul.h"
#include "GameStateEndOfGame.h"
#include "GameStateTraining.h"
#include "RestoreBall.h"

GameState * GameStateFactory::deserialize(std::string s)
{
    switch (s[0]) {
        case 'G' : return GameState::fromString(s);
        case 'R' : return GameStatePotRed::fromString(s);
        case 'C' : return GameStateChooseColor::fromString(s);
        case 'K' : return GameStatePotColor::fromString((s));
        case 'V' : return GameStateCleanColor::fromString(s);
        case 'F' : return GameStateFoul::fromString(s);
        case 'E' : return GameStateEndOfGame::fromString(s);
        case 'T' : return GameStateTraining::fromString(s);
        default  : return GameState::fromString("G");
    }
}

GameState * GameStateFactory::getFirstShotState(
        Player player, SnookerModel & model, bool whitePotted)
{
    const int blackId = 6;

    for (int i = blackId + 1; i < MAX_BALLS; i++)
        if (!model.positions[i].potted)
            return new GameStatePotRed(player, model, whitePotted);

    for (int i = ballValue(YELLOW_BALL); i <= ballValue(BLACK_BALL); i++)
        if (!model.positions[i-1].potted)
            return new GameStateCleanColor(player, model, i, whitePotted);
    
    if (model.result.points[0] > model.result.points[1])
        return new GameStateEndOfGame(Player(0), model);
    else if (model.result.points[0] < model.result.points[1])
        return new GameStateEndOfGame(Player(1), model);
    else {
        RestoreBall::restoreWhiteBall(model);
        model.positions[blackId].potted = true;
        RestoreBall::restoreBall(model, blackId, BLACK_SPOT);
        return new GameStateCleanColor(player, model, ballValue(BLACK_BALL), true);
    }
}
