#include "MenuItem.h"

#include <OGRE/Overlay/OgreOverlayManager.h>

using namespace Ogre;

bool alwaysTrue(MenuItem *mi)
{
    return true;
}

MenuItem::MenuItem(
        std::string name,
        std::string templateName):
    onClose(alwaysTrue),
    private_data(NULL)
{
    if (templateName == "")
        templateName = "Snoo/MenuItem";
    overlayElement = dynamic_cast<TextAreaOverlayElement*>(
            OverlayManager::getSingleton().createOverlayElementFromTemplate(
                templateName, "TextArea", name));
}

MenuItem::~MenuItem()
{
}

int MenuItem::getHeight(void)
{
    return 25;
}

void MenuItem::setWidth(int w)
{
    overlayElement->setWidth(w);
}

void MenuItem::setTop(int top)
{
    overlayElement->setTop(top);
}

int MenuItem::getTop(void)
{
    return overlayElement->getTop();
}

void MenuItem::setParentMenu(Menu *menu)
{
    parentMenu = menu;
}
        
Menu * MenuItem::getParentMenu(void)
{
    return parentMenu;
}

Ogre::TextAreaOverlayElement * MenuItem::getOverlayElement(void)
{
    return overlayElement;
}

void MenuItem::setTopColor(float r, float g, float b)
{
    overlayElement->setColourTop(ColourValue(r, g, b));
}

void MenuItem::setBottomColor(float r, float g, float b)
{
    overlayElement->setColourBottom(ColourValue(r, g, b));
}

void MenuItem::setColor(float r, float g, float b)
{
    overlayElement->setColour(ColourValue(r, g, b));
}

