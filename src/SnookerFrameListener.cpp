#include "OgreRenderWindow.h"
#include "OgreCamera.h"
#include "SnookerFrameListener.h"
#include "SnookerApplication.h"

using namespace Ogre;

SnookerFrameListener::SnookerFrameListener(
        SnookerApplication *app,
        Ogre::RenderWindow *wnd,
        Ogre::Camera *cam,
        OIS::KeyListener &keyListener,
        OIS::MouseListener &mouseListener):
    FrameListenerFramework(app, wnd, cam, keyListener, mouseListener)
{
}

SnookerFrameListener::~SnookerFrameListener() {
}

bool SnookerFrameListener::frameStarted(const FrameEvent &event) {
    keyboard->capture();
    mouse->capture();
    application->updateScene(event.timeSinceLastFrame);
    return application->isRunning();
}
