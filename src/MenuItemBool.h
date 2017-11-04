#ifndef __MenuItemBool_h__
#define __MenuItemBool_h__

#include "MenuItemLabel.h"

class MenuItemBool: public MenuItemLabel {
    public:
        MenuItemBool(std::string, std::string, std::string, std::string, bool *);
        virtual ~MenuItemBool();
        virtual void setCaption(std::string caption);
        virtual void injectKey(OIS::KeyCode);
    private:
        std::string genCaption(void);
        std::string caption, trueStr, falseStr;
        bool *value;
};

#endif
