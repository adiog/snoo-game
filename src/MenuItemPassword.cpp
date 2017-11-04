#include "MenuItemPassword.h"

MenuItemPassword::MenuItemPassword(std::string name, std::string * lbl):
    MenuItemEdit(name, lbl)
{
    update();
}

MenuItemPassword::~MenuItemPassword()
{
}

void MenuItemPassword::update(void)
{
    *label = text.getString();
    std::string asterisks = "> ";
    for (unsigned int i = 0; i < label->size(); i++)
        asterisks += '*';
    MenuItemLabel::setCaption(std::string(asterisks));
}
