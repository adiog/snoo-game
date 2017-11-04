#ifndef __BallCushionCollision_h__
#define __BallCushionCollision_h__

#include "Cushion.h"
#include "BallPosition.h"
#include "Collision.h"

class BallCushionCollision: public Collision {
    public:
        BallCushionCollision(double, BallPosition &, Cushion &);
        virtual ~BallCushionCollision() {}
        void apply(void);
    private:
        BallPosition & ball;
        Cushion & cushion;
};

#endif
