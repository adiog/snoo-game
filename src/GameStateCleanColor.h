#ifndef __GameStateCleanColor_h__
#define __GameStateCleanColor_h__

#include "GameState.h"
#include "SnooCommons.h"

class GameStateCleanColor: public GameState {
    public:
        GameStateCleanColor(Player, SnookerModel &, int, bool = false);
        Decisions getAvailableDecisions(SnookerModel &);
        GameState * applyShot(ShotEvents &, SnookerModel &, std::string &);
        bool canShot(void);
        std::string description(void);
        
        /// Serializacja
        std::string toString(void);
        static GameState * fromString(std::string);
    protected:
        int whichColor;
        GameStateCleanColor();
};

#endif
