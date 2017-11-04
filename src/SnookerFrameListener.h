#ifndef __SnookerFrameListener_h__
#define __SnookerFrameListener_h__

#include "OgreRenderWindow.h"
#include "OgreCamera.h"
#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include "FrameListenerFramework.h"

class SnookerFrameListener: public FrameListenerFramework {
    public:
        SnookerFrameListener(
                class SnookerApplication *app,
                Ogre::RenderWindow *wnd,
                Ogre::Camera *cam,
                OIS::KeyListener &keyListener,
                OIS::MouseListener &mouseListener);
        virtual ~SnookerFrameListener();
        bool frameStarted(const Ogre::FrameEvent &event);
};

#endif
