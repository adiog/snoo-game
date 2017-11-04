#include "CushionFace.h"

#include "R3.h"
#include "SnooCommons.h"
#include "BallPosition.h"

CushionFace::CushionFace(const R3 _a, const R3 _b):
    a(_a), b(_b)
{
}

CushionFace::~CushionFace()
{
}

bool CushionFace::collidesWith(const BallPosition & ball) const
{
    // Najpierw sprawdzamy, czy bila leci na bandę, czy od bandy.
    // Jeśli się od bandy oddala, to ignorujemy to zderzenie
    bool approaches = ((b - a).crossProd(ball.v))[2] > 0;
    if (!approaches)
        return false;

    // S - pozycja srodka bili
    R3 S = ball.pos;

    // TODO jesli bila lata, to my to ignorujemy
    S[2] = 0.0;

    // Sprawdzamy, czy konce bandy nie leza w srodku bili
    if (S.distanceTo(a) <= BALL_RADIUS)
        return true;
    if (S.distanceTo(b) <= BALL_RADIUS)
        return true;

    // Wektor wzdluz czola bandy
    R3 AB = b - a;

    // Wektor od poczatku czola bandy do srodka bili
    R3 AS = S - a;
    
    // C - punk na odcinku AB, przez ktory przechodzi prosta
    // prostopadla do AB i przechodzaca przez srodek bili (S)
    R3 AC = AS.projected(AB);
    R3 C = a + AC;
    
    // Jesli srodek bili jest oddalony za bardzo od czola bandy, to jest zle
    if ((AS - AC).length() > BALL_RADIUS)
        return false;

    // Srodek bandy
    R3 middle = a + (AB / 2);

    // Jesli C jest oddalone od srodka bandy wiecej niz |AB| / 2, to jest zle
    if (C.distanceTo(middle) > (AB.length() / 2))
        return false;

    return true;
}

double CushionFace::distanceTo(const BallPosition & ball) const
{
    // TODO: Uwaga: liczymy odleglosc wzdluz normalnej, a powinnismy liczyc wzdluz predkosci bili

    // a, b - konce bandy, S - srodek bili. C - punkt na AB, przez ktory przechodzi
    // prostopadla do AB przecinajaca punkt S
    R3 AB = b - a;
    R3 AS = b - ball.pos;
    R3 SC = AS.projected(AB) - AS;
    return (a + AS + (SC.unit() * BALL_RADIUS)).distanceTo(a + AS + SC);
}

R3 CushionFace::calculateNormal(const BallPosition & ball) const
{
    R3 normal = (b - a).crossProd(R3(0,0,-1)).unit();
    return normal;
}
