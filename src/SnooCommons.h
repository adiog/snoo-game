#ifndef __SnooCommons_h__
#define __SnooCommons_h__

#include <string>
#include "R3.h"


/* Ten plik zawiera definicje różnych rzeczy przydatnych
 * w całej grze, główne geometrię stołu. Wszystkie stałe
 * oznaczające długość podane są w centymetrach.
 */

const double PI = 3.14159265358979323846264338327;

/// Promień bili 
const double BALL_RADIUS = 2.6204;

// masa bili
#define BALL_M 1
// moment bezwladnosci bili
#define BALL_I ((0.4)*BALL_M*BALL_RADIUS*BALL_RADIUS)
// tarcie kinetyczne
#define MU 20.0
// grawitacja
#define GRAV 9.82
// wspolczynnik tarcia dla toczenia bez poslizgu
#define MU_ROLL 3.0
#define MU_ZAXIS 75
// minimalna predkosc dla ktorej bila sie porusza
#define V_MIN R3(0.0001)
// wartosc predkosci wzgledem podloza przy ktorej zaczyna sie poslizg
#define SLIDE_SPEED (R3(1.0) * MU * GRAV * 5.0 * 0.0003)
// ramie sily tarcia
#define Z_BALL_RADIUS R3(0.0, 0.0, -BALL_RADIUS)
// wspolczynnik sily uderzenia 
#define FORCE_FACTOR 100

/// Współrzędne środka niebieskiej bili
const R3 TABLE_CENTER = R3(0, 0, 0);

/// Długość stołu (częśći kamiennej, nie jest liczona część drewniana)
const double TABLE_LENGTH = 360.0;

/// Szerokość stołu (j.w.)
const double TABLE_WIDTH = 180.0;

/// Szerokość gumowej części band
const double CUSHION_WIDTH = 6.3;

/// Odległość między krótkimi bandami
const double PLAYAREA_LENGTH = TABLE_LENGTH - 2 * CUSHION_WIDTH; 

/// Odległość między długimi bandami
const double PLAYAREA_WIDTH = TABLE_WIDTH - 2 * CUSHION_WIDTH;

/// Odległość linii bazy od krawędzi stołu
const double BASELINE_POS = 73.7 + CUSHION_WIDTH;

/// Promień pola D
const double D_RADIUS = 29.2;

/// Promień otworu kieszeni narożnej
const double POCKET_RADIUS_CORNER = 8.7;

/// Promień otworu kieszeni środkowej
const double POCKET_RADIUS_MIDDLE = 5.2;

/// Pozycja bili brązowej
const R3 BROWN_SPOT = TABLE_CENTER - R3(0, TABLE_LENGTH / 2 - BASELINE_POS);

/// Pozycja bili żółtej
const R3 YELLOW_SPOT = BROWN_SPOT + R3(D_RADIUS, 0);

/// Pozycja bili zielonej
const R3 GREEN_SPOT = BROWN_SPOT - R3(D_RADIUS, 0);

/// Pozycja bili niebieskiej
const R3 BLUE_SPOT = TABLE_CENTER;

/// Pozycja bili różowej
const R3 PINK_SPOT = BLUE_SPOT + R3(0, PLAYAREA_LENGTH / 4);

/// Pozycja bili czarnej
const R3 BLACK_SPOT = BLUE_SPOT + R3(0, PLAYAREA_LENGTH / 2 - 32.4);

/// Ilość bil w grze
const int MAX_BALLS = 22;

/// Stałe odpowiadające typom bil
typedef enum {
    WHITE_BALL = 0,
    YELLOW_BALL,
    GREEN_BALL,
    BROWN_BALL,
    BLUE_BALL,
    PINK_BALL,
    BLACK_BALL,
    RED_BALL,
    NO_BALL
} BallType;

int ballValue(BallType);
std::string ballName(BallType);

/** Pozycje początkowe czerwonych bil
 * @param id 0 <= id < 15
 */
R3 initialRedPosition(const int id);

std::string intToString(int i);

#endif
