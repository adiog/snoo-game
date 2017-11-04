#ifndef __CushionFace_h__
#define __CushionFace_h__

#include "R3.h"
#include "BallPosition.h"
#include "Cushion.h"

class CushionFace: public Cushion {
    public:
        /** Początek i koniec czoła bandy, patrząc na stół od góry
         mają być one w kierunku "zgodnie z ruchem wskazówek zegara" */
        CushionFace(const R3, const R3);
        virtual ~CushionFace();
        
        /** Czy banda koliduje z daną bilą. */
        virtual bool collidesWith(const BallPosition &) const;

        /** Jak głęboko bila jest "wbita" w bandę - o tyle trzeba cofnąć bilę,
         aby się stykała z bandą. */
        virtual double distanceTo(const BallPosition &) const;

        /** Wektor prostopadły do bandy o początku w punkcie styku bili z bandą.
         Założenie: bila styka się z bandą */
        virtual R3 calculateNormal(const BallPosition &) const;
    private:
        R3 a;
        R3 b;
};

#endif
