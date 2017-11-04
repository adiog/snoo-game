#include "GameObjectFactory.h"

#include <sstream>
#include "OgreSceneManager.h"
#include "OgreSubEntity.h"

#include "GameObject.h"
#include "Ball.h"
#include "BallPosition.h"
#include "SnooCommons.h"

using namespace std;
using namespace Ogre;

GameObjectFactory::GameObjectFactory() {
    manager = NULL;
}

GameObjectFactory *GameObjectFactory::getSingleton(void) {
    static GameObjectFactory singleton;
    return &singleton;
}

void GameObjectFactory::setManager(SceneManager *m) {
    manager = m;
}

GameObject * GameObjectFactory::createObject(const string &name, const string &resource) {
    if (manager == NULL)
        throw NoSceneManagerException();
    return createObject(name, resource, manager->getRootSceneNode());
}

GameObject * GameObjectFactory::createObject(const string &name, const string &resource, GameObject *parent) {
    if (manager == NULL)
        throw NoSceneManagerException();
    return createObject(name, resource, parent->node);
}

GameObject * GameObjectFactory::createObject(const string &name, const string &resource, SceneNode *parent) {
    SceneNode *node = parent->createChildSceneNode();
    Entity *entity = manager->createEntity(name, resource);
    GameObject *object = new GameObject(node, entity);

    entity->setCastShadows(true);
    node->attachObject(entity);
    
    return object;
}

Ball * GameObjectFactory::createBall(GameObject *parent, BallPosition *position) {
    int id = position->id;

    std::ostringstream ballName;
    ballName << "Ball" << id;
    if (id == -1)
        id = 0;
    
    std::ostringstream materialName;
    materialName << "MaterialBall" << ballValue(typeOfBall(id));
    
    SceneNode *node = manager->getRootSceneNode()->createChildSceneNode();
    Entity *entity = manager->createEntity(ballName.str(), "Ball.mesh");
    entity->setCastShadows(true); 
    entity->setMaterialName(materialName.str());
    node->attachObject(entity);
    //entity->getSubEntity(0)->getMaterial()->getTechnique(0)->getPass(0)->setPolygonMode(PM_WIREFRAME);
    
    Ball *ball = new Ball(node, entity, position);
    return ball;
}
