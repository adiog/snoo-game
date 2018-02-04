#ifndef __FrameListenerFramework_h__
#define __FrameListenerFramework_h__

#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgreWindowEventUtilities.h>
#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

#include "ApplicationFramework.h"


class FrameListenerFramework: public Ogre::FrameListener, public Ogre::WindowEventListener {
    public:
        FrameListenerFramework(
                class ApplicationFramework *app,
                Ogre::RenderWindow *wnd,
                Ogre::Camera *cam,
                OIS::KeyListener &keyListener,
                OIS::MouseListener &mouseListener);
        virtual ~FrameListenerFramework();
        virtual void windowResized(Ogre::RenderWindow *wnd);
        virtual void windowClosed(Ogre::RenderWindow *wnd);
        bool frameStarted(const Ogre::FrameEvent &event);
        OIS::Keyboard * getKeyboard(void);
        OIS::Mouse * getMouse(void);
    protected:
        class ApplicationFramework * application;
        Ogre::Camera * camera;
        Ogre::RenderWindow * window;
        OIS::InputManager * inputManager;
        OIS::Mouse * mouse;
        OIS::Keyboard * keyboard;
};


#endif
