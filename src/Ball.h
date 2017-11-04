#ifndef __Ball_h__
#define __Ball_h__

#include "GameObject.h"
#include "OgreSceneNode.h"
#include "BallPosition.h"

class Ball: public GameObject {
    public:
        Ball(Ogre::SceneNode *, Ogre::Entity *, BallPosition *);
        virtual ~Ball();
        void updatePosition();
    private:
        BallPosition *position;
};

#endif
