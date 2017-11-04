#ifndef __GameStateEndOfGame_h__
#define __GameStateEndOfGame_h__

#include "GameState.h"

class GameStateEndOfGame: public GameState {
    public:
        GameStateEndOfGame(Player, SnookerModel &);
        bool isFinished(void);
        std::string description(void);
        
        /// Serializacja
        std::string toString(void);
        static GameState * fromString(std::string);
    protected:
        GameStateEndOfGame();
};

#endif
