#include "OgreSceneNode.h"
#include "OgreEntity.h"

#include "GameObject.h"

using namespace Ogre;

GameObject::GameObject(Ogre::SceneNode *n, Ogre::Entity *e) {
    node = n;
    entity = e;
}

GameObject::~GameObject() {
}
