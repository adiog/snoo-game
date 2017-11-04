#include "BallCushionCollision.h"
#include "Cushion.h"
#include "R3.h"
#include "SnooCommons.h"

BallCushionCollision::BallCushionCollision(
        double t, BallPosition & _ball, Cushion & _cushion):
    Collision(t), ball(_ball), cushion(_cushion)
{
}

void BallCushionCollision::apply(void)
{
#define CUSHION_LOSS 0.9
#define MU_BOUND (MU / 100.0)
#define COLLISION_TIME 0.1

    // normalna bandy
    R3 normal = cushion.calculateNormal(ball).unit();

    // efektywna predkosc wzgledem bandy
    R3 veff = ball.v + ball.w.crossProd(normal) * BALL_RADIUS;


    // sila nacisku na bande
    R3 vn = ball.v.projected(normal);
    R3 fn = vn * BALL_M * (1.0 + CUSHION_LOSS) / COLLISION_TIME;

    // a - przyspieszenie wynikajace z sil tarcia
    R3 a = - (veff-veff.projected(normal)).unit() * MU_BOUND * fn.length();
    // moment sily
    R3 M = (normal * BALL_RADIUS).crossProd(a * BALL_M);
    // przyspieszenie katowe
    R3 e = M / BALL_I;

    // zmiana predkosci
    R3 dv = a * COLLISION_TIME;
    R3 dw = e * COLLISION_TIME;

    R3 dw2 = dw + ball.w.projected(dw);

    // czy przyspieszenia wplynie na zwrot predkosci katowej
    if (dw2.dotProd(ball.w) < 0) {
        dw -= dw2;
        dv = dv.unit() * dw.length() * BALL_I / BALL_M / BALL_RADIUS;
    } 

#ifdef DEBUG
    std::cout << "dv " << dv << "  >> dw " << dw << std::endl;
#endif
    
    ball.v += dv;
    ball.w += dw;
    ball.v[2] = 0.0;

    ball.v -= ball.v.projected(normal) * 2;
    ball.v *= CUSHION_LOSS;
    ball.w *= CUSHION_LOSS;

#ifdef DEBUG
    std::cout << "po V: " << ball.v << std::endl;
    std::cout << "po W: " << ball.w << std::endl;
#endif
 
}

