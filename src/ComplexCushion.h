#ifndef __ComplexCushion_h__
#define __ComplexCushion_h__

#include <vector>

#include "R3.h"
#include "BallPosition.h"
#include "Cushion.h"

class ComplexCushion: public Cushion {
    public:
        ComplexCushion();
        virtual ~ComplexCushion();

        /** Dodaje bandę do grupy, przejmuje obiekt na własność,
         sam zwolni pamięć jak będzie potrzeba */
        virtual void add(Cushion *);
        
        /** Czy banda koliduje z daną bilą. */
        virtual bool collidesWith(const BallPosition &) const;

        /** Jak głęboko bila jest "wbita" w bandę - o tyle trzeba cofnąć bilę,
         aby się stykała z bandą. */
        virtual double distanceTo(const BallPosition &) const;

        /** Wektor prostopadły do bandy o początku w punkcie styku bili z bandą.
         Założenie: bila styka się z bandą */
        virtual R3 calculateNormal(const BallPosition &) const;

    private:
        std::vector<Cushion *> elements;

};

#endif
