#include "R3.h"
#include "Ball.h"
#include "BallPosition.h"
#include "Converter.h"
#include "OgreEntity.h"
#include "OgreSceneNode.h"
#include "OgreVector3.h"
#include "Orientation.h"
#include "SnooCommons.h"

using namespace Ogre;

Ball::Ball(SceneNode *n, Entity *e, BallPosition *p)
    : GameObject(n, e), position(p) {
    updatePosition();
}

Ball::~Ball() {}

void Ball::updatePosition() {
    node->setVisible(!position->potted);
    if (!position->potted) {
        node->setPosition(Converter::toOgre(position->pos));
        node->setOrientation(Converter::toOgre(position->orientation));
    }
}
