#ifndef __Popup_h__
#define __Popup_h__

#include <string>
#include "OgreOverlay.h"

class Popup {
    public:
      Popup(std::string, std::string);
      virtual ~Popup();
    private:
      Ogre::Overlay * overlay;
      static int order;
};

#endif
