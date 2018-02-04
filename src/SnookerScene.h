#ifndef __SnookerScene_h__
#define __SnookerScene_h__

#include <string>
#include <OGRE/OgreLight.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgrePanelOverlayElement.h>
#include "GameObject.h"
#include "SnookerModel.h"
#include "Ball.h"
#include "BallPosition.h"
#include "SnooCommons.h"


class SnookerScene {
    public:
        SnookerScene(Ogre::SceneManager *, SnookerModel *);
        virtual ~SnookerScene();
        virtual void update(void);
        virtual void updateHint(void);
        void setAccOvlVisible(bool);
        void setAccOvlText(int, std::string);
        void toggleWireframe();
    private:
        bool wireframeMode;
        bool showHintArrow;
        int hintArrowMaterialId;
        SnookerModel * model;
        GameObject * frame;
        GameObject * table;
        GameObject * cushions;
        GameObject * additions;
        GameObject * cue;
        Ball * balls[MAX_BALLS];
        Ball * hint;
        GameObject * hintArrow;
        BallPosition hintPosition;
        Ogre::Light * light1, * light2;
        Ogre::Overlay * acc_ovl;

        Ogre::Light * createLamp(Ogre::SceneManager *, std::string, R3);
};

#endif
