#include "Physics.h"

#include <iostream>
#include <cmath>
#include "SnookerModel.h"
#include "Table.h"
#include "BallPositions.h"
#include "BallPosition.h"
#include "Collision.h"
#include "BallBallCollision.h"
#include "BallCushionCollision.h"
#include "R3.h"
#include "SnooCommons.h"

const double Physics::resolution(0.0003);

static inline void round_double(double & d)
{
}

void Physics::initShot(SnookerModel &model)
{
    model.begin = model.positions;
    model.time = model.lastTime = 0.0;
    model.shotEvents.removeAll();
    
    // Obliczamy prędkość początkową bili na podstawie kierunku i wartości
    double fx = cos(model.tilt) * cos(model.vectorDirection);
    double fy = cos(model.tilt) * sin(model.vectorDirection);
    double fz = -sin(model.tilt);

    // Sila
    R3 f = R3(fx,fy,fz) * model.vectorSize * BALL_M * FORCE_FACTOR;

    // Ustawiamy prędkość początkową
    model.positions[0].v = f * R3(1.0, 1.0, 0.0) / BALL_M;

    // Ramię 
    double tmpz = sqrt(1 - model.whiteBallPoint.lengthSquared());
    if (tmpz != tmpz) tmpz = 0.0;
    R3 r = R3(-tmpz, -model.whiteBallPoint[0], -model.whiteBallPoint[1]);
    
    r = r.rotate(R3(0.0, model.tilt, 0.0));
    r = r.rotate(R3(0.0, 0.0, model.vectorDirection)); 
    r *= BALL_RADIUS;

    // Moment sily
    R3 m = r.crossProd(f); 

    // Początkowa prędkość kątowa
    model.positions[0].w = m / BALL_I;

    round_double(model.positions[0].w[0]);
    round_double(model.positions[0].w[1]);
    round_double(model.positions[0].w[2]);
    round_double(model.positions[0].v[0]);
    round_double(model.positions[0].v[1]);
    round_double(model.positions[0].v[2]);
        
    model.shotInProgress = true;
}

void Physics::updateModel(SnookerModel &model, double dTime)
{
    double nextTime = model.lastTime + dTime;
    

    // obliczamy pozycje bil do momentu, gdy time >= nextTime
    while (model.time < nextTime)
        calculateNext(model);

    model.lastTime = nextTime;
}

void Physics::checkPockets(BallPosition & ball)
{
    const double W = TABLE_WIDTH;
    const double L = TABLE_LENGTH;

    if (ball.potted)
        return;

    for (int i = -1; i <= 1; i+=2) {
        for (int j = -1; j <= 1; j+=2)
            if (ball.pos.distanceTo(R3(i*(W/2 - 1.5), j*(L/2 - 1.5))) < POCKET_RADIUS_CORNER) {
                ball.potted = true;
                ball.v = R3::ZERO;
                ball.w = R3::ZERO;
                generateEvent(ShotEvent(BALL_IN_POCKET_EVENT, typeOfBall(ball.id)));
                return;
            }
        if (ball.pos.distanceTo(R3(i*(W/2 - 0.4), 0)) < POCKET_RADIUS_MIDDLE) {
                ball.potted = true;
                ball.v = R3::ZERO;
                ball.w = R3::ZERO;
                generateEvent(ShotEvent(BALL_IN_POCKET_EVENT, typeOfBall(ball.id)));
                return;
            }
    }    
}

void Physics::updatePosition(BallPosition & ball, double dTime)
{
    if (ball.potted) {
        ball.v = R3::ZERO;
        ball.w = R3::ZERO;
    } else {
        if (ball.v.absolute() < V_MIN && 
            ball.w.absolute() * BALL_RADIUS < V_MIN) {
            // predkosci zaniedbywalnie male
            ball.v = R3::ZERO;
            ball.w = R3::ZERO;
        } else { 
            // prędkości przy których modyfikujemy współczynniki

            // veff - efektywna predkosc wzgledem podloza
            R3 veff = ball.v + ball.w.crossProd(Z_BALL_RADIUS);
            // a - przyspieszenie wynikajace z sil tarcia
            R3 a = veff.unit() * MU * GRAV;
            // moment sily
            R3 M = Z_BALL_RADIUS.crossProd(a) * BALL_M;
            // przyspieszenie katowe
            R3 e = M / BALL_I;

            // tarcie wokol osi Z
            double dw = (ball.w[2] > 0 ? 1 : -1) * MU_ZAXIS * dTime;
            if (fabs(dw) > fabs(ball.w[2]))
                ball.w[2] = 0.0;
            else
                ball.w[2] -= dw;
            double w2 = ball.w[2];

            // graniczna wartosc bliska odpowiadajaca toczeniu bez poslizgu
            if (veff.absolute() < SLIDE_SPEED) {
                // zwykle toczenie bez poslizgu
                // v ~ wr, zwalniamy predkosc liniowo  

                //  wyliczamy nowe tarcie
                R3 acc = ball.v.unit() * MU_ROLL * GRAV;

                if ((acc * dTime).length() >= ball.v.length()) {
                    ball.v = R3::ZERO;
                    ball.w = R3::ZERO;
                } else {
                    ball.v -= acc * dTime ;
                    ball.w = Z_BALL_RADIUS.crossProd(-ball.v) / (BALL_RADIUS * BALL_RADIUS);
                }

            } else {
                // toczenie z poslizgiem
		        R3 dv = a * dTime;
    		    R3 dw = e * dTime;
	
				// wygladzenie hamowania
                // veff2 - nowa efektywna predkosc wzgledem podloza
                R3 veff2 = ball.v + dv + (ball.w + dw).crossProd(Z_BALL_RADIUS);
        		// czy przyspieszenia wplynie na zwrot predkosci katowej
	    	    if (veff.dotProd(veff2) < 0) {
		            ball.w = Z_BALL_RADIUS.crossProd(-ball.v) / (BALL_RADIUS * BALL_RADIUS); 
		        } else {
                    ball.v -= dv;
                    ball.w -= dw;
                }
           }

            ball.w[2] = w2;
        }

    ball.pos += ball.v * dTime;
    ball.orientation.rotateBy(ball.w * dTime);
    round_double(ball.pos[0]);
    round_double(ball.pos[1]);
    round_double(ball.pos[2]);
    round_double(ball.v[0]);
    round_double(ball.v[1]);
    round_double(ball.v[2]);
    round_double(ball.w[0]);
    round_double(ball.w[1]);
    round_double(ball.w[2]);
    checkPockets(ball);
    }
}

void Physics::updatePositions(BallPositions &positions, double dTime)
{
    for (int ballId = 0; ballId < MAX_BALLS; ballId++)
        updatePosition(positions[ballId], dTime);
}

Collision * Physics::getFirstCollision(BallPositions & positions, double time)
{
    Cushion & cushion = Table::getSingleton().getCushions();
    const double R = BALL_RADIUS;
    Collision * collision = NULL;

    // Zderzenia bil z bandami
    for (int ballId = 0; ballId < MAX_BALLS; ballId++) {
        BallPosition & ball = positions[ballId];

        if (ball.potted || ball.v.length() == 0)
            continue;
        
        if (cushion.collidesWith(ball)) {
            // Obliczamy rzeczywisty moment zderzenia
            double t = time - cushion.distanceTo(ball) / ball.v.length();
            
            // Jeśli zderzenie ma szanse być pierwszym - zapamiętujemy
            if (collision == NULL || t < collision->time) {
                if (collision != NULL)
                    delete collision;
                collision = new BallCushionCollision(time, ball, cushion);
            }
        }
    }

    // Zderzenia bil z bilami
    for (int ballId = 0; ballId < MAX_BALLS; ballId++) {
        BallPosition & ballA = positions[ballId];

        if (ballA.potted)
            continue;

        for (int secBallId = ballId + 1; secBallId < MAX_BALLS; secBallId++) {
            BallPosition & ballB = positions[secBallId];

            // Nie uznajemy kolizji z bilami wbitymi
            if (ballB.potted)
                continue;
            
            // Kolizję ignorujemy, jeśli bile oddalają się od siebie
            if (!ballA.pos.approaches(ballA.v - ballB.v, ballB.pos))
                continue;

            // Kolizji nie ma też, gdy bile sa za daleko
            if (ballA.pos.distanceTo(ballB.pos) > 2 * BALL_RADIUS)
                continue;

            // OK - mamy kolizję, policzmy jej rzeczywisty czas i ew. zapamietajmy:
            double t = time;
            if (collision == NULL || t < collision->time) {
                if (collision != NULL)
                    delete collision;
                collision = new BallBallCollision(time, ballA, ballB);
            }
        }
    }

    return collision;
}

void Physics::calculateNext(SnookerModel &model)
{
    updatePositions(model.positions, resolution);
    model.time += resolution;

    Collision * collision;
    collision = getFirstCollision(model.positions, model.time);
    if (collision != NULL) {
        collision->apply();
        if (collision->isEvent())
            generateEvent(collision->getEvent());
        delete collision;
    }
}
