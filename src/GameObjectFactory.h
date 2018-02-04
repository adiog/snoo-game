#ifndef __GameObjectFactory_h__
#define __GameObjectFactory_h__

#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>

#include "GameObject.h"
#include "BallPosition.h"
#include "Ball.h"
#include "SnooCommons.h"

class NoSceneManagerException {};

class GameObjectFactory {
    public:
        static GameObjectFactory * getSingleton(void);
        void setManager(Ogre::SceneManager *m);
        GameObject * createObject(const std::string &name, const std::string &resource);
        GameObject * createObject(const std::string &name, const std::string &resource, GameObject *parent);
        Ball * createBall(GameObject *parent, BallPosition *position);
    private:
        Ogre::SceneManager *manager;
        GameObjectFactory();
        GameObject * createObject(const std::string &name, const std::string &resource, Ogre::SceneNode *parent);
};

#endif
