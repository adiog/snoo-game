#ifndef __ApplicationFramework_h_
#define __ApplicationFramework_h_

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreString.h>
#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

#include "FrameListenerFramework.h"

class ApplicationFramework {
    public:
        ApplicationFramework();
        virtual ~ApplicationFramework();
        virtual void go(void);
        virtual void close(void);
        virtual bool isRunning(void);
        virtual int getWidth(void);
        virtual int getHeight(void);
        virtual void updateScene(Ogre::Real dtime) = 0;
        OIS::Keyboard * getKeyboard(void);
        OIS::Mouse * getMouse(void);
    protected:
        Ogre::Root *root;
        Ogre::Camera *camera;
        Ogre::SceneManager *sceneManager;
        Ogre::RenderWindow *renderWindow;
        Ogre::String resourcePath;
        class FrameListenerFramework *frameListener;
        bool running;
        virtual bool setup(void);
        virtual bool configure(void);
        virtual void chooseSceneManager(void);
        virtual void createCamera(void);
        virtual void createFrameListener(void) = 0;
        virtual void createViewports(void);
        virtual void setupResources(void);
        virtual void createResourceListener(void);
        virtual void loadResources(void);
        virtual void setupGUI(void);
        virtual void createScene(void) = 0;
        virtual void destroyScene(void);
};

#endif
