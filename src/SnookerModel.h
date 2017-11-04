#ifndef __SnookerModel_h__
#define __SnookerModel_h__

#include <string>

#include "BallPositions.h"
#include "R3.h"
#include "Result.h"
#include "GameState.h"
#include "ShotEvents.h"

class GameState;

/// Klasa zawierajaca informacje o trwającej rozgrywce
class SnookerModel {
    public: 
        SnookerModel();
        virtual ~SnookerModel();
     
        /// Pozycje bil
        BallPositions positions;

        /// Czas od początku uderzenia, dla którego są obliczone pozycje
        double time;

        /// Czas od początku uderzenia, dla którego były ostatnio liczone pozycje
        double lastTime;

        /// Pozycje bil z poczatku ostatniego uderzenia
        BallPositions begin;

        /// Czy właśnie jest obliczane jakies uderzenie?
        bool shotInProgress;

        /// Informacja o wyniku trwającej rozgrywki
        Result result;

        /// Stan gry
        GameState * gameState;

        /// Stan gry przed poprzednim uderzeniem, zserializowany
        std::string beginGameState;

        /// Zdarzenia, które zaszły podczas ostatniego uderzenia
        ShotEvents shotEvents;

        /// Kierunek wektora wskazujący, w którą
        /// stonę będzie uderzona aktualna bila
        double vectorDirection;

        /// Kąt nachylenia kija nad białą bilą
        double tilt;

        /// Siła uderzenia
        double vectorSize;

        /// Punkt na bialej bili, w ktory celujemy,
        /// współrzędna z wektora ignorowana,
        /// jego dlugosc jest mniejsza badz rowna od 1,
        /// (0,0) to uderzenie w srodek
        R3 whiteBallPoint;

        /// Kierunek uderzena
        R3 getDirectionAsVector(void);

        /// Czy jakaś bila się rusza
        bool anyBallMoving(void);

        /// Zeruje predkosci wszystkich bil
        void initBallVelocity(void);

        /// Ustawia bile na pozycjach początkowych
        void initBallPositions(void);

        /// Ustawia model na początku gry
        void startNewGame(void);

        /// Ustawia pozycje bil z zserializowanenych pozycji
        void setPositionsFromString(const std::string &);

        /// Serializuje dane o uderzeniu
        std::string shotDataToString();

        /// Ustawia dane strzalu z serializowanych
        void setShotDataFromString(const std::string &);
};

#endif
