#include "MenuItemEdit.h"

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

#include "SnookerApplication.h"

using namespace OIS;

MenuItemEdit::MenuItemEdit(std::string n, std::string * s):
    MenuItemLabel(n, ""),
    label(s)
{
    setCaption(*s);
    setSelectable(true);
}

MenuItemEdit::~MenuItemEdit()
{
}

void MenuItemEdit::setCaption(std::string s)
{
    text.setString(s);
    update();
}

void MenuItemEdit::injectKey(OIS::KeyCode code)
{
    Keyboard * keyboard = SnookerApplication::getSingleton().getKeyboard();
    bool shift = keyboard->isKeyDown(KC_LSHIFT) || keyboard->isKeyDown(KC_RSHIFT);

    switch(code) {
        case KC_LEFT:
        case KC_RIGHT:
        case KC_RETURN:
        case KC_ESCAPE:
            MenuItemLabel::injectKey(code);
            break;
        default:
            text.injectKey(code, shift);
            update();
            break;
    }
}

void MenuItemEdit::update(void)
{
    *label = text.getString();
    MenuItemLabel::setCaption("> " + (*label));
}
