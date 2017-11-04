#ifndef __BallBallCollision_h__
#define __BallBallCollision_h__

#include "Collision.h"
#include "BallPosition.h"
#include "ShotEvent.h"

class BallBallCollision: public Collision {
    public:
        BallBallCollision(double, BallPosition &, BallPosition &);
        virtual ~BallBallCollision() {}
        void apply(void);
        ShotEvent getEvent(void);
    private:
        BallPosition & firstBall;
        BallPosition & secondBall;
};

#endif
