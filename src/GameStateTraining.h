#ifndef __GameStateTraining_h__
#define __GameStateTraining_h__

#include "GameState.h"

class GameStateTraining: public GameState {
    public:
        GameStateTraining(Player, SnookerModel &);
        Decisions getAvailableDecisions(SnookerModel &);
        GameState * applyShot(ShotEvents &, SnookerModel &, std::string &);
        bool canShot(void);
        std::string description(void);
        
        /// Serializacja
        std::string toString(void);
        static GameState * fromString(std::string);
    protected:
        GameStateTraining();
};

#endif
