#ifndef __GameObject_h__
#define __GameObject_h__

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreEntity.h>

class GameObject {
    public:
        GameObject(Ogre::SceneNode *, Ogre::Entity *);
        virtual ~GameObject();
        Ogre::Entity *entity;
        Ogre::SceneNode *node;
};

#endif
