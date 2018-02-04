#include "OverlayCommons.h"

#include <OGRE/Overlay/OgreOverlayManager.h>

#include "Id.h"

using namespace Ogre;

TextAreaOverlayElement * createTextArea(std::string templateName)
{
    return dynamic_cast<TextAreaOverlayElement*>(
            OverlayManager::getSingletonPtr()->createOverlayElementFromTemplate(
                templateName, "TextArea", Id::generate()));    
}


PanelOverlayElement * createPanel(std::string templateName)
{
    return dynamic_cast<PanelOverlayElement*>(
            OverlayManager::getSingletonPtr()->createOverlayElementFromTemplate(
                templateName, "Panel", Id::generate()));    
}

TextAreaOverlayElement * getTextArea(std::string name)
{
    return dynamic_cast<TextAreaOverlayElement*>(
            OverlayManager::getSingletonPtr()->getOverlayElement(name));
}

PanelOverlayElement * getPanel(std::string name)
{
    return dynamic_cast<PanelOverlayElement*>(
            OverlayManager::getSingletonPtr()->getOverlayElement(name));
}
