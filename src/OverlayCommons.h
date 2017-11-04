#ifndef _OverlayCommons_h_
#define _OverlayCommons_h_

#include <string>

#include "OgreTextAreaOverlayElement.h"
#include "OgrePanelOverlayElement.h"


Ogre::TextAreaOverlayElement * createTextArea(std::string);
Ogre::PanelOverlayElement * createPanel(std::string);

Ogre::TextAreaOverlayElement * getTextArea(std::string);
Ogre::PanelOverlayElement * getPanel(std::string);

#endif
