#include "MenuItemCommand.h"

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include "MenuItemLabel.h"

using namespace OIS;

MenuItemCommand::MenuItemCommand(
        std::string name,
        std::string caption,
        MenuItemCallback callback):
    MenuItemLabel(name, caption),
    onReturn(callback)
{
    setSelectable(true);
}

MenuItemCommand::~MenuItemCommand()
{
}

void MenuItemCommand::injectKey(KeyCode code)
{
    switch (code) {
        case KC_RIGHT:
        case KC_SPACE:
        case KC_RETURN:
            onReturn(this);
            break;
        default:
            break;
    }
}
