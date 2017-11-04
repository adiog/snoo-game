#include "MenuItemLabel.h"

MenuItemLabel::MenuItemLabel(std::string name, std::string caption):
    MenuItem(name, "")
{
    setSelectable(false);
    setCaption(caption);
}

MenuItemLabel::~MenuItemLabel()
{
}

void MenuItemLabel::setCaption(std::string caption)
{
    overlayElement->setCaption(caption);
}

std::string MenuItemLabel::getCaption(void)
{
    return overlayElement->getCaption();
}

void MenuItemLabel::setSelectable(bool s)
{
    selectable = s;
    deselect();
}

void MenuItemLabel::select(void)
{
    setColor(1, 1, 1);
}

void MenuItemLabel::deselect(void)
{
    if (selectable) {
        setTopColor(0, 1, 0);
        setBottomColor(1, 0, 0);
    } else {
        setColor(0.5, 0.5, 0.5);
    }
}

bool MenuItemLabel::isSelectable(void)
{
    return selectable;
}

void MenuItemLabel::injectKey(OIS::KeyCode code)
{
    return;
}
