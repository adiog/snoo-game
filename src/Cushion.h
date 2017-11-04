#ifndef __Cushion_h__
#define __Cushion_h__

#include "R3.h"
#include "BallPosition.h"

class Cushion {
    public:
        Cushion() { }

        virtual ~Cushion() { }

        /** Czy banda koliduje z daną bilą. */
        virtual bool collidesWith(const BallPosition &) const = 0;

        /** Jak głęboko bila jest "wbita" w bandę - o tyle trzeba cofnąć bilę,
         aby się stykała z bandą. */
        virtual double distanceTo(const BallPosition &) const = 0;

        /** Wektor prostopadły do bandy o początku w punkcie styku bili z bandą.
         Założenie: bila styka się z bandą */
        virtual R3 calculateNormal(const BallPosition &) const = 0;
};

#endif
