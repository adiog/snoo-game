#ifndef __MenuItemCommand_h__
#define __MenuItemCommand_h__

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include "Menu.h"
#include "MenuItemLabel.h"

class MenuItemCommand: public MenuItemLabel {
    public:
        MenuItemCommand(std::string, std::string, MenuItemCallback);
        virtual ~MenuItemCommand();
        virtual void injectKey(OIS::KeyCode);
    private:
        MenuItemCallback onReturn;

};

#endif
