#ifndef __Orientation_h__
#define __Orientation_h__

#include <iostream>
#include "R3.h"

class Orientation {
    public:
        Orientation();
        Orientation(double, double, double, double);
        virtual ~Orientation();

        /** Dostęp do 4 współrzędnych kwaternionu */
        double & operator[] (const int);
        
        /** Dostęp do 4 współrzędnych kwaternionu */
        const double & operator[] (const int) const;

        /** Obraca orientację o kąt w przestrzeni R3.
         Kąt to wektor, którego kierunek to oś obrotu,
         Zwrot wyznacza w którą stronę będzie obrót,
         Długość wektora jest równa kątowi
         */
        void rotateBy(R3);

        friend std::ostream & operator<<(std::ostream &, const Orientation &);
    private:
        double q[4];
};

#endif
