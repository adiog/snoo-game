#ifndef __GameStatePotRed_h__
#define __GameStatePotRed_h__

#include "GameState.h"

class GameStatePotRed: public GameState {
    public:
        GameStatePotRed(Player, SnookerModel &, bool = false);
        Decisions getAvailableDecisions(SnookerModel &);
        GameState * applyShot(ShotEvents &, SnookerModel &, std::string &);
        bool canShot(void);
        std::string description(void);
        
        /// Serializacja
        std::string toString(void);
        static GameState * fromString(std::string);
    protected:
        GameStatePotRed();
};

#endif
