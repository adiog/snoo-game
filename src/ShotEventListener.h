#ifndef __ShotEventListener_h__
#define __ShotEventListener_h__

#include "ShotEvent.h"

class ShotEventListener {
    public:
        virtual ~ShotEventListener() { }
        virtual void handleShotEvent(const ShotEvent &) = 0;
};

#endif
