#ifndef __GameStatePotColor_h__
#define __GameStatePotColor_h__

#include "GameState.h"
#include "SnooCommons.h"

class GameStatePotColor: public GameState {
    public:
        GameStatePotColor(Player, SnookerModel &, int, bool = false);
        Decisions getAvailableDecisions(SnookerModel &);
        GameState * applyShot(ShotEvents &, SnookerModel &, std::string &);
        bool canShot(void);
        std::string description(void);
        
        /// Serializacja
        std::string toString(void);
        static GameState * fromString(std::string);
    protected:
        int whichColor;
        GameStatePotColor();
};

#endif
