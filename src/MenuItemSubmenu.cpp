#include "MenuItemSubmenu.h"

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include "MenuItemLabel.h"


MenuItemSubmenu::MenuItemSubmenu(
        std::string name,
        std::string caption,
        Menu * menu):
    MenuItemLabel(name, caption),
    subMenu(menu)
{
    setSelectable(true);
}

MenuItemSubmenu::~MenuItemSubmenu()
{
}

void MenuItemSubmenu::injectKey(OIS::KeyCode code)
{
    using namespace OIS;

    switch (code) {
        case KC_RIGHT:
        case KC_SPACE:
        case KC_RETURN:
            subMenu->setTop(getTop());
            subMenu->setLeft(parentMenu->getLeft() + parentMenu->getWidth());
            subMenu->open(&parentMenu->subMenu);
            break;
        default:
            break;
    }
}
