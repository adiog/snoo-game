#ifndef __acc_protocol_h__
#define __acc_protocol_h__
#include "R3.h"

struct acc_msg {
	unsigned int type;	/* numer komunikatu */
	unsigned int arg;	  /* argument - bitmapa lub liczba w zapisie sieciowym */
	R3 argf;	          /* argumenty rzeczywiste */
};

/*** NUMERY POLECEN ***/
#define A_NOTHING 0
/* gra -> kontroler */
#define A_SET		1
#define A_GET		2
#define A_RESET 3       /* zerowanie wektorkow, pozycje te co A_SET_A*, tylko pozycja i predkosc */
#define A_NOTCHANGING 4 /* najm. 3 bity - nie zmienia sie orientacja wokol kolejnych osi, kolejne - predkosc wzdloz osi = 0 */
#define A_GESTURES	8	/* wlaczenie przechwytywania gestow (tylko z wlaczonym sampling) */

/* kontroler -> gra */
#define A_STATUS	32
#define A_KEYBCHG	33	/* arg - mapa klawiszy */
#define A_VBATT		34	/* arg - napiecie w mV */
#define A_ACC		35	/* argf - odczyty z akcelerometrow */
#define A_VEL		36	/* argf - biezace predkosci */
#define A_POS		37	/* argf - biezaca pozycja w przestrzeni */
#define A_ROT		38	/* argf - obrot (orientacja) w przestrzeni */
#define A_GESTURE	39	/* arg - nr gestu, argf - atrybuty */

/* argumenty polecenia A_SET */
/* najmlodsze 8 bitow: czestotliwosc wysylania probek w Hz */
#define A_SET_POWEROFF	(1<<8)
#define A_SET_RANGE	(1<<9)	/* 1 - 6G, 0 - 1.5G */
#define A_SET_SAMPLING	(1<<10) /* probkowanie wlaczone, mozna odczytac parametry */
#define A_SET_KEYBON	(1<<11) /* 1 - zmiana stanu klawiatury automatycznie przekazywana */
#define A_SET_APOS	(1<<12) /* automatyczne wysylanie pozycji */
#define A_SET_AVEL	(1<<13)
#define A_SET_AACC	(1<<14)
#define A_SET_AROT	(1<<15)

/* argumenty polecenia A_GET (poszczegolne parametry na zadanie) */
#define A_GET_STATUS	(1<<0)	/* wymusza przyslanie komunikatu A_STATUS */
#define A_GET_ACC	(1<<1)
#define A_GET_VEL	(1<<2)
#define A_GET_POS	(1<<3)
#define A_GET_ROT	(1<<4)
#define A_GET_KEYB	(1<<5)

/* numery gestow z opisami znaczenia atrybutow */
#define A_GESTURE_SHOT (1<<0)
#define A_GESTURE_MOVE_X (1<<1)
#define A_GESTURE_MOVE_Y (1<<2)
#define A_GESTURE_MOVE_ANY (1<<3)
#define A_GESTURE_MENUON (1<<4)
#define A_GESTURE_MENUOFF (1<<5)

#define A_GESTURE_ALL 0xffffffff

/* numery przyciskow */
#define A_KEY_FIRE  (1<<2)
#define A_KEY_UP    (1<<4)
#define A_KEY_DOWN  (1<<0)
#define A_KEY_LEFT  (1<<3)
#define A_KEY_RIGHT (1<<1)
#define A_KEY_NONE  0

#endif

