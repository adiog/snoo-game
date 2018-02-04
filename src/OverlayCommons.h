#ifndef _OverlayCommons_h_
#define _OverlayCommons_h_

#include <string>

#include <OGRE/Overlay/OgreTextAreaOverlayElement.h>
#include <OGRE/Overlay/OgrePanelOverlayElement.h>


Ogre::TextAreaOverlayElement * createTextArea(std::string);
Ogre::PanelOverlayElement * createPanel(std::string);

Ogre::TextAreaOverlayElement * getTextArea(std::string);
Ogre::PanelOverlayElement * getPanel(std::string);

#endif
