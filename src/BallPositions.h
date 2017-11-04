#ifndef __BallPositions_h__
#define __BallPositions_h__

#include "BallPosition.h"
#include "SnooCommons.h"

class BallPositions {
    public:
        BallPositions();
        BallPosition & operator[](const int);

        /// Zamienia pozycje bil na napis (np. do przesłania przez sieć)
        std::string toString(void);

        /// Ustawia pozycje bil na podstawie napisu
        void setFromString(const std::string &);
    private:
        BallPosition positions[MAX_BALLS];
};

#endif
