#ifndef __RestoreBall_h__
#define __RestoreBall_h__

#include "R3.h"
#include "SnookerModel.h"

class RestoreBall {
    public:
        
        /** Sprawdza, czy w danym miejscu jest miejsce na postawienie bili */
        static bool canPlaceBall(SnookerModel &, R3);

        /** Wstawia kolorową bilę o danym id na dane miejsce,
         * lub inne zgodnie z zasadmi */
        static void restoreBall(SnookerModel &, int, R3);
        
        /** Wyjmuje z kieszeni i ustawia białą bilę w polu D */
        static void restoreWhiteBall(SnookerModel &);

};

#endif
