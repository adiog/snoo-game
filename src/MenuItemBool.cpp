#include "MenuItemBool.h"

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

using namespace OIS;

MenuItemBool::MenuItemBool(
        std::string name,
        std::string caption,
        std::string tStr,
        std::string fStr,
        bool *valuePtr):
    MenuItemLabel(name, caption)
{
    falseStr = fStr;
    trueStr = tStr;
    value = valuePtr;
    setSelectable(true);
    setCaption(caption);
}

MenuItemBool::~MenuItemBool()
{
}


std::string MenuItemBool::genCaption()
{
    if (*value)
        return caption + ": " + trueStr;
    else
        return caption + ": " + falseStr;
}

void MenuItemBool::setCaption(std::string capt)
{
    caption = capt;
    MenuItemLabel::setCaption(genCaption());
}

void MenuItemBool::injectKey(OIS::KeyCode code)
{
    switch(code) {
        case KC_SPACE:
        case KC_RIGHT:
        case KC_RETURN:
            *value = ! (*value);
            setCaption(caption);
            break;
        default:
            break;
    }
}
