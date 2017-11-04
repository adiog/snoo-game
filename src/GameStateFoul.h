#ifndef __GameStateFoul_h__
#define __GameStateFoul_h__

#include "GameState.h"

class GameStateFoul: public GameState {
    public:
        GameStateFoul(Player, SnookerModel &, bool, bool);
        Decisions getAvailableDecisions(SnookerModel &);
        GameState * applyDecision(Decision &, SnookerModel &, std::string &);
        std::string description(void);
        
        /// Serializacja
        std::string toString(void);
        static GameState * fromString(std::string);
    protected:
        bool miss;
        GameStateFoul();
};

#endif
