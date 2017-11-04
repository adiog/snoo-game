#ifndef __MenuItemEdit_h__
#define __MenuItemEdit_h__

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include "Menu.h"
#include "MenuItemLabel.h"
#include "OISString.h"

class MenuItemEdit: public MenuItemLabel {
    public:
        MenuItemEdit(std::string, std::string *);
        virtual ~MenuItemEdit();
        virtual void setCaption(std::string);
        virtual void injectKey(OIS::KeyCode);
    protected:
        virtual void update(void);
        std::string * label;
        OISString text;
};

#endif
