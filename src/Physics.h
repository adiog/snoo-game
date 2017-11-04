#ifndef __Physics_h__
#define __Physics_h__

#include "SnookerModel.h"
#include "BallPositions.h"
#include "Collision.h"
#include "ShotEventGenerator.h"

/**
 * Klasa odpowiedzialna za fizykę gry - oblicznie pozycji
 * bil w czasie uderzeń
 */
class Physics: public ShotEventGenerator {
    public:
        /** Odstęp czasowy, co jaki będa obliczane nowe pozycje bil.
         Pozycje bil pomiędzy momentami oddalonymi co resolution sekund
         są interpolowane. */
        static const double resolution;

        /** Rozpoczyna uderzenie (wprawia w ruch białą bilę)
         na podstawie informacji zawartych w modelu */
        virtual void initShot(SnookerModel &model);
        
        /** Uaktualnia model tak, aby positions przedstawiało pozycję bil
         po upłynięciu kolejnej chwili czasu
         @param model model do uaktualnienia
         @param dTime liczba sekund o jaką mają się uaktualnić pozycje positions
         */
        virtual void updateModel(SnookerModel &model, double dTime);

    private:
        /** Sprawdza, czy bila nie trafila wlasnie do jakiejs kieszeni i jesli
            tak jest - ustawia potted na true */
        void checkPockets(BallPosition & ball);

        /** Przesuwa jedną bilę o odległość pokonaną w czasie dTime,
            uaktulania rotację */
        void updatePosition(BallPosition & ball, double dTime);

        /** Przesuwa bile o odległość pokonaną w czasie dTime, ignoruje kolizje */
        void updatePositions(BallPositions & positions, double dTime);

        /** Zwraca pierwsze zderzenie, lub NULL jeśli og nie było */
        Collision * getFirstCollision(BallPositions & positions, double time);

        /** Dostaje pozycje bil w chwili zderzenia. Aplikuje dane o zderzeniu */
        void applyCollision(BallPositions & positions, Collision & collision);
        
        /** Oblicza pozycje bil w modelu po upłynięciu czasu resolution */
        void calculateNext(SnookerModel &model);
};

#endif
