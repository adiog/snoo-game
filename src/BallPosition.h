#ifndef __BallPosition_h__
#define __BallPosition_h__

#include "R3.h"
#include "SnooCommons.h"
#include "Orientation.h"

BallType typeOfBall(int id);

class BallPosition {
    public:
        BallPosition();
        BallPosition(R3 &initial);
        virtual ~BallPosition();

        /** Identyfikator bili */
        int id;
        
        /** Czy bila jest wbita */
        bool potted;

        /** Pozycja bili. Wektor - współrzędna z jest ignorowana. 
         Pozycja (0,0) jest w rogu stołu przy zielonej bili,
         współrzędne x rosną w stronę czarnej bili, y w stronę żółtej.
         Uwaga: Pozycja o współrzędnej 0 jest tam, gdzie drewniana
         część bandy styka się z gumową. Wymiary band, stołu itp.
         w pliku include/SnooCommons.h
         */
        R3 pos;

        /** Orientacja bili */
        Orientation orientation;

        /** Prędkość bili, współrzędna z ignorowana */
        R3 v;
        
        /** Prędkość kątowa bili: wektor którego kierunek to oś obrotu,
         długość to wartość prędkości kątowej (rad/s)
         */
        R3 w;
};

#endif
