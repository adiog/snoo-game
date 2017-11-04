#ifndef __GameStateChooseColor_h__
#define __GameStateChooseColor_h__

#include "GameState.h"

class GameStateChooseColor: public GameState {
    public:
        GameStateChooseColor(Player, SnookerModel &);
        Decisions getAvailableDecisions(SnookerModel &);
        GameState * applyDecision(Decision &, SnookerModel &, std::string &);
        std::string description(void);
        
        /// Serializacja
        std::string toString(void);
        static GameState * fromString(std::string);
    protected:
        GameStateChooseColor();
};

#endif
