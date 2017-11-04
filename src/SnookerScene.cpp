#include "SnookerScene.h"

#include <sstream>
#include "OgreSceneNode.h"
#include "OgreLight.h"
#include "OgreSceneManager.h"
#include "OgreSubEntity.h"
#include "OgreOverlayManager.h"
#include "OgrePanelOverlayElement.h"
#include "Ogre.h"
#include "OgreTextAreaOverlayElement.h"

#include "GameObjectFactory.h"
#include "SnookerModel.h"
#include "SnooCommons.h"
#include "SnookerApplication.h"
#include "Converter.h"
#include "Table.h"

using namespace Ogre;

SnookerScene::SnookerScene(Ogre::SceneManager *manager, SnookerModel *m)
{
    GameObjectFactory *factory = GameObjectFactory::getSingleton();
    factory->setManager(manager);
    
    model = m;
    
    manager->setAmbientLight(ColourValue(0.2, 0.2, 0.2));
    manager->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

    R3 position = TABLE_CENTER - R3(0, 0, 1.0 + BALL_RADIUS);
    
    table = factory->createObject("Table", "Blat.mesh");
    // additions = factory->createObject("Adds", "Narozniki.mesh");
    cushions = factory->createObject("Cushions", "Bandy.mesh");
    frame = factory->createObject("Frame", "Rama.mesh");
    hintArrow = factory->createObject("HintArrow", "Arrow.mesh");
    hintArrow->entity->setCastShadows(false);
    showHintArrow = false;
    hintArrowMaterialId = 0;
    
    table->node->setPosition(Converter::toOgre(position));
    cushions->node->setPosition(Converter::toOgre(position));
    frame->node->setPosition(Converter::toOgre(position));
    // additions->node->setPosition(Converter::toOgre(position));

    cue = factory->createObject("Cue", "Cue.mesh");
    cue->entity->setCastShadows(true);
   
    wireframeMode = false;
    for (int i = 0; i < MAX_BALLS; i++)
        balls[i] = factory->createBall(table, &model->positions[i]);
    
    hintPosition.id = -1;
    hint = factory->createBall(table, &hintPosition);

    model->initBallPositions();
    
    light1 = createLamp(manager, "SomeLight", R3(0, 0, 200));
    //light1 = createLamp(manager, "SomeLight2", R3(-100, 0, 200));

    light2 = manager->createLight("OtherLight");
    light2->setPowerScale(3.0);
    light2->setType(Light::LT_DIRECTIONAL);
    light2->setDirection(0, -1, 0);
    light2->setDiffuseColour(ColourValue(0.5, 0.5, 0.5));
    light2->setSpecularColour(ColourValue(0.5, 0.5, 0.5));
    
    SnookerApplication & application = SnookerApplication::getSingleton();
    OverlayManager * overlayManager = OverlayManager::getSingletonPtr();
    acc_ovl = overlayManager->getByName("Snoo/ACC");
    overlayManager->getOverlayElement("ACC")->setLeft(
            application.getWidth()-325);

}

SnookerScene::~SnookerScene()
{
    delete cue;
    for (int i = 0; i < MAX_BALLS; i++)
        delete balls[i];
    //delete table;
    //delete frame;
}

Light * SnookerScene::createLamp(SceneManager * manager, std::string name, R3 pos)
{
    Light * light = manager->createLight(name);
    light->setType(Light::LT_POINT);
    light->setPosition(Converter::toOgre(pos));
    light->setDiffuseColour(ColourValue(1, 1, 1));
    light->setSpecularColour(ColourValue(1, 1, 1));
    return light;
}

void SnookerScene::update(void)
{
    for (int i = 0; i < MAX_BALLS; i++) {
        balls[i]->updatePosition();
    }

    double & angle = model->vectorDirection;
    double & tilt = model->tilt;
    
    // Ramię - wektor od środka bili do punktu, w który celujemy
    R3 r;
    double d;
    if (model->whiteBallPoint.lengthSquared() >= 1)
        d = 0.0;
    else
        d = sqrt(1 - model->whiteBallPoint.lengthSquared());
    r = R3(-d, -model->whiteBallPoint[0], -model->whiteBallPoint[1]);
    r *= BALL_RADIUS;
    r = r.rotate(R3(0.0, tilt, 0.0));
    r = r.rotate(R3(0.0, 0.0, angle));

    // Kierunek, w którym kijowi jest "do przodu"
    R3 direction;
    double fx = cos(tilt) * cos(angle);
    double fy = cos(tilt) * sin(angle);
    double fz = -sin(tilt);
    direction = R3(fx, fy, fz).unit();

    // Odległość kija od powierzchni bili w kierunku, w krórym się on porusza
    R3 distance = direction * (model->vectorSize * 10.0 + 5.0);
   
    hint->node->setVisible(SnookerApplication::getSingleton().canShot());
    hintArrow->node->setVisible(showHintArrow && SnookerApplication::getSingleton().canShot());

    cue->node->setVisible(SnookerApplication::getSingleton().anyoneCanShot());
    cue->node->setPosition(balls[0]->node->getPosition());
    cue->node->resetOrientation();
    cue->node->roll(Radian(tilt));
    cue->node->yaw(Radian(angle + PI / 2), Node::TS_WORLD);
    cue->node->translate(Converter::toOgre(r - distance));
}

void SnookerScene::setAccOvlVisible(bool s)
{
    OverlayManager * overlayManager = OverlayManager::getSingletonPtr();
    if(s)
        overlayManager->getByName("Snoo/ACC")->show();
    else
        overlayManager->getByName("Snoo/ACC")->hide();
}

void SnookerScene::setAccOvlText(int n, std::string s)
{
    std::ostringstream elemname;
    elemname << "ACC/Label" << n;
    OverlayManager::getSingletonPtr()->getOverlayElement(elemname.str())->setCaption(s);
}

void SnookerScene::updateHint(void)
{
    SnookerModel & model = SnookerApplication::getSingleton().model;
    Cushion & cushion = Table::getSingleton().getCushions();

    hintPosition.pos = model.positions[0].pos;
    bool collision = false;
    R3 vect(cos(model.vectorDirection), sin(model.vectorDirection));
    vect = vect.unit() * 0.05;
    hintPosition.v = vect;

    int k = 5;
    int ball = 0;
    
    while (k > 0) {
        hintPosition.pos += vect;
        for (int i = 1; i < MAX_BALLS; i++) {
            if (model.positions[i].potted)
                continue;
            if (hintPosition.pos.distanceTo(model.positions[i].pos) < 
                    2 * BALL_RADIUS) {
                collision = true;
                ball = i;
            }
        }
        
        if (cushion.collidesWith(hintPosition)) {
            collision = true;
            ball = 0;
        }
        
        if (hintPosition.pos.distanceTo(model.positions[0].pos) > 370) {
            collision = true;
            ball = 0;
        }
        
        if (collision) {
            hintPosition.pos -= vect;
            vect /= 10.0;
            k--;
        }
    }

    if (ball == 0) {
        // hint nie zatrzymal sie na zadnej z bil
        showHintArrow = false;
    } else {
        showHintArrow = true;

        hintArrow->node->resetOrientation();
        R3 v = R3(0, 1, 0); // w ta strone wskazuje teraz hint

        // a w ta strone ma wskazywac:
        R3 w = model.positions[ball].pos - hintPosition.pos;
        double cos_alpha = v.dotProd(w) / v.length() / w.length();
        double alpha = acos(cos_alpha);
        if (v.crossProd(w)[2] < 0)
            alpha *= -1;
        Orientation o;
        o.rotateBy(R3(0, 0, alpha));
        
        R3 w2 = hintPosition.pos - model.positions[0].pos;
        double hintLen = w2.dotProd(w) / (w2.length() + 0.001) / w.length();
        hintLen *= hintLen * 100;
        hintLen *= (1 - 2 * w2.length() / TABLE_LENGTH);
        if (hintLen < 0.5)
            hintLen = 0.5;
        
        double thickness = 0.3;
        if (SnookerApplication::getSingleton().cameraMode == SnookerApplication::CAMERA_MODE_TOP)
            thickness = 2;
        if (hintArrowMaterialId != ballValue(typeOfBall(model.positions[ball].id))) {
            std::ostringstream materialName;
            hintArrowMaterialId = ballValue(typeOfBall(model.positions[ball].id));
            materialName << "ArrowMat" << hintArrowMaterialId;
            hintArrow->entity->getSubEntity(0)->setMaterialName(materialName.str());
        }
        hintArrow->node->setScale(hintLen, 1, 2);
        hintArrow->node->setOrientation(Converter::toOgre(o));
        hintArrow->node->setPosition(Converter::toOgre(
                    model.positions[ball].pos - R3(0, 0, BALL_RADIUS + 1.0 - thickness)));
    }


    hint->updatePosition();
}

void SnookerScene::toggleWireframe(void)
{
    wireframeMode = !wireframeMode;
    for (int i = 0; i < MAX_BALLS; i++) {
        std::ostringstream materialName;
        materialName << "MaterialBall" << ballValue(typeOfBall(i));
        if (wireframeMode)
            materialName << "w";
        balls[i]->entity->getSubEntity(0)->setMaterialName(materialName.str());
    }
}
