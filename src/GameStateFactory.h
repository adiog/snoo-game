#ifndef __GameStateFactory_h__
#define __GameStateFactory_h__

#include <string>
#include "GameState.h"
#include "SnookerModel.h"

class GameStateFactory {
    public:
        static GameState * deserialize(std::string);

        /**
            Zwraca stan, w którym zaczyna się podejście do stołu,
            tj. PotRed jeśli są czerwone, lub odpowiedni inny gdy ich nie ma
         */
        static GameState * getFirstShotState(
                Player p, SnookerModel &, bool whitePotted = false);
};

#endif
