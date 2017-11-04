#ifndef __CushionDisk_h__
#define __CushionDisk_h__

#include "R3.h"
#include "BallPosition.h"
#include "Cushion.h"

class CushionDisk: public Cushion {
    public:
        CushionDisk(const R3, const double);
        virtual ~CushionDisk();
        
        /** Czy banda koliduje z daną bilą. */
        virtual bool collidesWith(const BallPosition &) const;

        /** Jak głęboko bila jest "wbita" w bandę - o tyle trzeba cofnąć bilę,
         aby się stykała z bandą. */
        virtual double distanceTo(const BallPosition &) const;

        /** Wektor prostopadły do bandy o początku w punkcie styku bili z bandą.
         Założenie: bila styka się z bandą */
        virtual R3 calculateNormal(const BallPosition &) const;
    private:
        R3 s;
        double r;
};

#endif
