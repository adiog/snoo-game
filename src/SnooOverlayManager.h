#ifndef SNOO_SNOOOVERLAYSINGLETON_H
#define SNOO_SNOOOVERLAYSINGLETON_H


#include <Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OGRE/Overlay/OgreOverlayManager.h>


class SnooOverlayManager {
public:
    static Ogre::OverlaySystem* getOverlaySystem(){
        static std::unique_ptr<Ogre::OverlaySystem> overlaySystem = std::make_unique<Ogre::OverlaySystem>();
        return overlaySystem.get();
    }

    static Ogre::OverlayManager* getSingletonPtr(void)
    {
        //static std::unique_ptr<Ogre::OverlayManager> overlayManager = std::make_unique<Ogre::OverlayManager>();
        static Ogre::OverlaySystem* overlaySystem = getOverlaySystem();
        //return overlayManager.get();
        return Ogre::OverlayManager::getSingletonPtr();
    }

    static Ogre::OverlayManager& getSingleton(void)
    {
        return *SnooOverlayManager::getSingletonPtr();
    }
};


#endif //SNOO_SNOOOVERLAYSINGLETON_H
