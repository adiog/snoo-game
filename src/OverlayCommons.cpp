#include "OverlayCommons.h"

#include <OGRE/Overlay/OgreOverlayManager.h>

#include "Id.h"
#include "SnooOverlayManager.h"

using namespace Ogre;

TextAreaOverlayElement * createTextArea(std::string templateName)
{
    return dynamic_cast<TextAreaOverlayElement*>(
            SnooOverlayManager::getSingletonPtr()->createOverlayElementFromTemplate(
                templateName, "TextArea", Id::generate()));    
}


PanelOverlayElement * createPanel(std::string templateName)
{
    return dynamic_cast<PanelOverlayElement*>(
            SnooOverlayManager::getSingletonPtr()->createOverlayElementFromTemplate(
                templateName, "Panel", Id::generate()));    
}

TextAreaOverlayElement * getTextArea(std::string name)
{
    return dynamic_cast<TextAreaOverlayElement*>(
            SnooOverlayManager::getSingletonPtr()->getOverlayElement(name));
}

PanelOverlayElement * getPanel(std::string name)
{
    return dynamic_cast<PanelOverlayElement*>(
            SnooOverlayManager::getSingletonPtr()->getOverlayElement(name));
}
