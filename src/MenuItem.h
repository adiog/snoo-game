#ifndef __MenuItem_h__
#define __MenuItem_h__

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include "OgreTextAreaOverlayElement.h"
#include "Menu.h"
class Menu;

typedef bool (* MenuItemCallback) (class MenuItem *);

class MenuItem {
    public:
        MenuItem(std::string, std::string);
        virtual ~MenuItem();
        virtual void select(void) = 0;
        virtual void deselect(void) = 0;
        virtual bool isSelectable(void) = 0;
        virtual void injectKey(OIS::KeyCode) = 0;
        virtual int getHeight(void);
        virtual void setWidth(int);
        virtual void setTop(int);
        virtual int getTop(void);
        virtual void setParentMenu(Menu *);
        virtual Menu * getParentMenu(void);
        virtual Ogre::TextAreaOverlayElement * getOverlayElement(void);          
        MenuItemCallback onClose;
        void * private_data;
    protected:
        void setTopColor(float, float, float);
        void setBottomColor(float, float, float);
        void setColor(float, float, float);
        Menu * parentMenu;
        Ogre::TextAreaOverlayElement *overlayElement;
};

#endif
