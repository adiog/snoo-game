#ifndef __ShotEventGenerator_h__
#define __ShotEventGenerator_h__

#include <vector>

#include "ShotEvent.h"
#include "ShotEventListener.h"

class ShotEventGenerator {
    public:
        ~ShotEventGenerator();
        virtual void addListener(ShotEventListener *);
        virtual void generateEvent(const ShotEvent &);
    private:
        std::vector< ShotEventListener* > listeners;
};

#endif
