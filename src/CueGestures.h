#ifndef __CueGestures_h__
#define __CueGestures_h__

#include <queue>
#include "R3.h"
#include "acc_protocol.h"

using namespace std;

#define SAMPLES_NUM 300
#define SAMPLES_PER_LTAVG 25

#define GST_MOVE_MINTIME 10
#define GST_MOVE_MAXTIME 40
#define GST_MOVE_MINSWEEP 0.25

#define GST_SHOT_MINTIME 5
#define GST_SHOT_MAXTIME 80
#define GST_SHOT_MINSWEEP 0.1
#define GST_SHOT_MINHILL 60

#define GST_SHOT_MULTIPLIER 0.2

#define GST_MENUON_PREC 0.3
#define GST_MENUOFF_PREC 0.5

#define IS_MOVING_DELTA 0.1

#define OX 0
#define OY 1
#define OZ 2

class Gesture {
    public:
        Gesture() {}
        ~Gesture(){}
        int type;     // jak w acc_protocol.h
        int iattr;
        R3 fattr;
};

class CueGestures {
    public:
        CueGestures();
        ~CueGestures();
        
        void enable(int gesture);
        void disable(int gesture);
        void load(R3 sample);
        Gesture * get();
    private:
        int enabledGestures;
        R3 samples[SAMPLES_NUM];
        int nextSample;
        double integrate(int axis, int count, double bias);
        bool menuOpened;
        queue<Gesture *> gesturesQueue;
        R3 longTimeAvg;     // srednia dlugoterminowa (z SAMPLES_PER_LTAVG 
                            // probek)
        R3 sweepDir;        // odchylenie od sredniej, znak oznacza kierunek, 
                            // wartosc bezwzgl. - czas od zmiany kierunku
        R3 lastSweepLTA;    // srednia w momencie ostatniej zmiany kierunku
        R3 maxSweep;        // maksymalne odchylenia od sredniej od zm. kier.
        R3 lastHill;        // czas od ostatniej wykrytej gorki
};

/*
Rozpoznawanie prostych ruchow - gorek na wykresie dla danej osi:
  Gorka to czas od przejscia przez srednia do osiagniecia tej wartosci znow,
  trwajaca czas mieszczacy sie w okreslonym przedziale i z maksymalnym
  wychyleniem rownym conajmniej okreslona wartosc.
  Ewentualnie mozna cos podejrzanego o gorke interpolowac z 3 punktow
  (poczatek, koniec, max) i patrzec na blad, czy nie sa to jakies losowe
  drgania
*/

#endif
