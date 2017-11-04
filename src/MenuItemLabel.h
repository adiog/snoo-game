#ifndef __MenuItemLabel_h__
#define __MenuItemLabel_h__

#include "MenuItem.h"


class MenuItemLabel: public MenuItem {
    public:
        MenuItemLabel(std::string, std::string);
        virtual void select(void);
        virtual void deselect(void);
        virtual bool isSelectable(void);
        virtual void injectKey(OIS::KeyCode);
        virtual void setCaption(std::string);  ///< TODO: const string & 
        virtual void setSelectable(bool);
        virtual std::string getCaption(void);
        virtual ~MenuItemLabel();
    private:
        bool selectable;
};

#endif
