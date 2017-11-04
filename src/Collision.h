#ifndef __Collision_h__
#define __Collision_h__

#include "BallPosition.h"
#include "ShotEvent.h"

/// Wyjątek rzucany przez getEvent
class NoEventException {};

class Collision {
    public:
        /// Czas od początku oderzenia, w którym nastąpiło zderzenie
        double time;

        Collision(double);

        virtual ~Collision();

        /// Aplikuje zderzenie
        virtual void apply(void) = 0;

        /// Czy to zderzenie jest zdarzeniem w sensie ShotEvent?
        virtual bool isEvent(void);

        /// Zwraca ShotEvent, lub rzuca NoEventException()
        virtual ShotEvent getEvent(void);
};

#endif
