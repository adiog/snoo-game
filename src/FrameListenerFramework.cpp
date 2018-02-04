#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgreWindowEventUtilities.h>
#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

#include "FrameListenerFramework.h"

using namespace Ogre;

FrameListenerFramework::FrameListenerFramework(
        ApplicationFramework *app,
        RenderWindow *wnd,
        Camera *cam,
        OIS::KeyListener &keyListener,
        OIS::MouseListener &mouseListener) {
    using namespace OIS;
    
    ParamList paramList;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    
    window = wnd;
    camera = cam;
    application = app;
    
    window->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    inputManager = InputManager::createInputSystem(paramList);
    
    keyboard = dynamic_cast<Keyboard*> (
            inputManager->createInputObject(OISKeyboard, true));
    keyboard->setEventCallback(&keyListener);
    
    mouse = dynamic_cast<Mouse*> (
            inputManager->createInputObject(OISMouse, true));
    mouse->setEventCallback(&mouseListener);
    
    // Set initial clipping size
    windowResized(window);
    
    // Register as a Window listener
    WindowEventUtilities::addWindowEventListener(window, this);
}

FrameListenerFramework::~FrameListenerFramework() {
    WindowEventUtilities::removeWindowEventListener(window, this);
    windowClosed(window);
}

void FrameListenerFramework::windowResized(RenderWindow *wnd) {
    unsigned int width, height, depth;
    int left, top;

    wnd->getMetrics(width, height, depth, left, top);
    const OIS::MouseState &mouseState = mouse->getMouseState();
    mouseState.width = width;
    mouseState.height = height;
}

void FrameListenerFramework::windowClosed(RenderWindow *wnd) {
    if (wnd == window) {
        if (inputManager) {
            inputManager->destroyInputObject(mouse);
            inputManager->destroyInputObject(keyboard);
            OIS::InputManager::destroyInputSystem(inputManager);
            inputManager = NULL;
        }
    }
}

bool FrameListenerFramework::frameStarted(const FrameEvent &event) {
    keyboard->capture();
    mouse->capture();
    return application->isRunning();
}

OIS::Keyboard * FrameListenerFramework::getKeyboard(void)
{
    return keyboard;
}

OIS::Mouse * FrameListenerFramework::getMouse(void)
{
    return mouse;
}
