#ifndef __ShotEvent_h__
#define __ShotEvent_h__

#include "SnooCommons.h"

enum ShotEventType {
    BALL_HIT_EVENT,
    BALL_IN_POCKET_EVENT
};


/// Reprezentuje zdarzenie, które wystąpiło w trakcie uderzenia.
/// Na postawie takich zdarzeń zasady gry mówią o
/// zmianach w punktach i stanie gry
class ShotEvent {
    public:
        /// Konstuktor zdarzenia
        ShotEvent(ShotEventType t, BallType b): type(t), ball(b) { }

        /// Typ zdarzenia
        ShotEventType type;

        /// Bila, która wzięła udział w zdarzeniu
        BallType ball;
};

#endif
