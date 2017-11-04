#include "ComplexCushion.h"

#include <vector>

#include "R3.h"
#include "BallPosition.h"

ComplexCushion::ComplexCushion()
{
}

ComplexCushion::~ComplexCushion()
{
    for (unsigned int i = 0; i < elements.size(); i++)
        delete elements[i];
}

void ComplexCushion::add(Cushion * element)
{
    elements.push_back(element);
}

bool ComplexCushion::collidesWith(const BallPosition & ball) const
{
    for (unsigned int i = 0; i < elements.size(); i++)
        if (elements[i]->collidesWith(ball))
            return true;
    return false;
}

double ComplexCushion::distanceTo(const BallPosition & ball) const
{
    double distance = -1000000.0;

    for (unsigned int i = 0; i < elements.size(); i++) {
        double d = elements[i]->distanceTo(ball);
        if (d > distance)
            distance = d;
    }
    return distance;
}

R3 ComplexCushion::calculateNormal(const BallPosition & ball) const
{
    double minDistance = 1000000.0;
    int element = -1;

    for (unsigned int i = 0; i < elements.size(); i++) {
        double d = elements[i]->distanceTo(ball);
        if (fabs(d) < minDistance) {
            minDistance = d;
            element = i;
        }
    }

    if (element != -1)
        return elements[element]->calculateNormal(ball);
    else
        throw 78632;
}
