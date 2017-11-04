#ifndef __GameState_h__
#define __GameState_h__

#include <vector>
#include <string>
#include "Decision.h"
#include "ShotEvents.h"
#include "SnookerModel.h"
#include "Player.h"

class SnookerModel;

/// Ten wyjątek rzuca applyXxx, jeśli nie udało się zastosować strzału
/// lub decyzji w danym stanie gry
class NotApplicableException {};

class GameState {
    public:
        /// Tworzy stan w którym będzie grał dany gracz
        GameState(Player, SnookerModel &, bool = false);

        virtual ~GameState();

        /// Zwróć listę decycji, które można podjąć
        virtual Decisions getAvailableDecisions(SnookerModel &);

        /// Zaaplikuj podjętą przez gracza decyzję, zwróć nowy stan
        virtual GameState * applyDecision(Decision &, SnookerModel &, std::string &);

        /// Uaktulanij punktację, zwróć nowy stan
        virtual GameState * applyShot(ShotEvents &, SnookerModel &, std::string &);

        /// Aktualny gracz
        virtual Player getPlayer(void);

        /// Zmiana aktualnego gracza
        virtual void setPlayer(Player);

        /// Czy gracz może przesuwać białą bilę w polu D?
        virtual bool canMoveWhiteBall(void);

        /// Czy gracz może wykonać uderzenie?
        virtual bool canShot(void);

        /// Czy gra się już zakończyła?
        virtual bool isFinished(void);

        /// Słowny opis stanu
        virtual std::string description(void);

        /// Serializacja
        virtual std::string toString(void);
        static GameState * fromString(std::string);
    protected:
        GameState();

        Player player;

        /// Ustawione, jeśli w momencie tworzenia stanu biała bila była wbita
        bool wasWhiteBallPotted;

        /// Oblicz ile jest punktów za foul na danej bili
        int foulValue(BallType);

        /// Oblicz ile jest punktów za faul na danej bili, nie mniej niż dany int
        int newFaulValue(BallType, int);

        /// Przywraca kolorowe bile na nominaln epozycje
        void restoreColors(SnookerModel & model);
};

#endif
