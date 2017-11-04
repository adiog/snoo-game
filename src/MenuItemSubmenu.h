#ifndef __MenuItemSubmenu_h__
#define __MenuItemSubmenu_h__

#include "Menu.h"
#include "MenuItemLabel.h"

class MenuItemSubmenu: public MenuItemLabel {
    public:
        MenuItemSubmenu(std::string, std::string, Menu *);
        virtual void injectKey(OIS::KeyCode);
        virtual ~MenuItemSubmenu();
    private:
        Menu * subMenu;
};

#endif
