#ifndef __MenuItemPassword_h__
#define __MenuItemPassword_h__

#include "MenuItemEdit.h"

class MenuItemPassword: public MenuItemEdit {
    public:
        MenuItemPassword(std::string, std::string *);
        ~MenuItemPassword();
    protected:
        virtual void update(void);
};

#endif
