#include "ApplicationFramework.h"

#include <iostream>
#include <cstdlib>
#include "Runtime.h"
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreString.h>
#include <OGRE/Overlay/OgreOverlayManager.h>

using namespace Ogre;

ApplicationFramework::ApplicationFramework()
{
    frameListener = NULL;
    root = NULL;
    sceneManager = NULL;
    frameListener = NULL;
    renderWindow = NULL;
    running = true;
}

ApplicationFramework::~ApplicationFramework()
{
    if (frameListener)
        delete frameListener;
}

void ApplicationFramework::go(void)
{
    if (!setup()) {
        std::cerr << "Setup failed" << std::endl;
        return;
    }
    root->startRendering(); // run application
    destroyScene();                 // clean up
}

int ApplicationFramework::getWidth(void)
{
    if (renderWindow)
        return renderWindow->getWidth();
    else
        return 0;
}

int ApplicationFramework::getHeight(void)
{
    if (renderWindow)
        return renderWindow->getHeight();
    else
        return 0;
}

void ApplicationFramework::close(void)
{
    running = false;
}

bool ApplicationFramework::isRunning(void)
{
    return running;
}

bool ApplicationFramework::setup(void)
{
    root = new Root(
            "plugins.cfg",
            Runtime::workDir() + "ogre.cfg",
            Runtime::workDir() + "log");
    
    setupResources();
    if (!configure())
        return false;

    chooseSceneManager();
    createCamera();
    createViewports();
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    createResourceListener();
    loadResources();
    setupGUI();
    createScene();
    createFrameListener();
    return true;
}


bool ApplicationFramework::configure(void)
{
    if (root->showConfigDialog()) {
        renderWindow = root->initialise(true, "Snoo");
        return true;
    } else {
        return false;
    }
}

void ApplicationFramework::chooseSceneManager(void)
{
    sceneManager = root->createSceneManager(ST_GENERIC, "ExampleSMInstance");
}

void ApplicationFramework::createCamera(void)
{
    camera = sceneManager->createCamera("PlayerCam");
    camera->setPosition(Vector3(0, 0, 500));
    camera->lookAt(Vector3(0, 0, 0));
    camera->setNearClipDistance(0.01);
}

void ApplicationFramework::createViewports(void)
{
    Viewport *vp = renderWindow->addViewport(camera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    camera->setAspectRatio(
            Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

void ApplicationFramework::setupResources(void)
{
    ConfigFile cf;
    cf.load("resources.cfg");
    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    String secName, typeName, archName;

    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
        }
    }
}

void ApplicationFramework::createResourceListener(void)
{
}

void ApplicationFramework::loadResources(void)
{
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void ApplicationFramework::setupGUI(void)
{
}

void ApplicationFramework::destroyScene(void)
{
}

OIS::Keyboard * ApplicationFramework::getKeyboard(void)
{
    return frameListener->getKeyboard();
}

OIS::Mouse * ApplicationFramework::getMouse(void)
{
    return frameListener->getMouse();
}
