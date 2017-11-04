#ifndef __ShotEvents_h__
#define __ShotEvents_h__

#include <vector>
#include "SnooCommons.h"
#include "ShotEvent.h"

/// Reprezentuje kolejne zdarzenia, które
/// zaszły podczas jednego uderzenia
class ShotEvents {
    public:
        void addEvent(ShotEvent);
        void removeAll(void);
        std::vector<ShotEvent> & getEvents(void);
        int count(void);
        BallType firstBallHit(void);                
    private:
        std::vector<ShotEvent> events;
};

#endif
