#include "Orientation.h"
#include <cmath>
#include "R3.h"

Orientation::Orientation()
{
    // Kwaternion neutralny (brak obrotu) = (1,0,0,0)
    q[0] = 1;
    q[1] = q[2] = q[3] = 0.0;
}

Orientation::Orientation(double w, double x, double y, double z)
{
    q[0] = w;
    q[1] = x;
    q[2] = y;
    q[3] = z;
}

Orientation::~Orientation()
{
}

double & Orientation::operator[] (const int i)
{
    return q[i];
}

const double & Orientation::operator[] (const int i) const
{
    return q[i];
}

void Orientation::rotateBy(R3 vect)
{
    // Jeśli obracamy o kąt 0...
    if (vect.length() == 0.0)
        return;

    // Normalizujemy dany wektor tworząc oś obrotu
    double angle = vect.length();
    R3 axis = vect.unit();

    // Tworzymy kwaternion u na podstawie osi i kąta
    double u[4];
    u[0] = cos(0.5 * angle);
    u[1] = sin(0.5 * angle) * axis[0];
    u[2] = sin(0.5 * angle) * axis[1];
    u[3] = sin(0.5 * angle) * axis[2];

    // r <- u * this (mnożymy kwaterniony <=> składamy obroty)
    double r[4];
    r[0] = u[0] * q[0] - u[1] * q[1] - u[2] * q[2] - u[3] * q[3];
    r[1] = u[0] * q[1] + u[1] * q[0] + u[2] * q[3] - u[3] * q[2];
    r[2] = u[0] * q[2] + u[2] * q[0] + u[3] * q[1] - u[1] * q[3];
    r[3] = u[0] * q[3] + u[3] * q[0] + u[1] * q[2] - u[2] * q[1];

    // this <- u
    for (int i = 0; i < 4; i++)
        q[i] = r[i];
}

std::ostream & operator<<(std::ostream & out, const Orientation & o)
{
    out << o[0] << " " << o[1] << " " << o[2] << " " << o[3];
    return out;
}
