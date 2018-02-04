#ifndef __Menu_h__
#define __Menu_h__

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include "MenuItem.h"
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgrePanelOverlayElement.h>
class MenuItem;

class Menu {
    public:
        Menu(std::string);
        
        Menu ** parentsChild;
        Menu * subMenu;
        
        virtual ~Menu();
        virtual MenuItem * addMenuItem(MenuItem *);
        virtual void injectKey(OIS::KeyCode);
        
        virtual int getHeight(void);
        virtual void setHeight(int);
        
        virtual int getWidth(void);
        virtual void setWidth(int);
        
        virtual int getTop(void);
        virtual void setTop(int);
        
        virtual int getLeft(void);
        virtual void setLeft(int);
        
        virtual bool getVisible(void);
        virtual void setVisible(bool);
        
        virtual void open(Menu **);
        virtual void close(void);
    private:
        bool hasSelectableItem();

        std::vector<MenuItem*> items;
        int position;
        bool selected;
        int width;
        int height;
        int top;
        int left;
        bool visible;
        Ogre::Overlay *overlay;
        Ogre::PanelOverlayElement * topPanel;
        Ogre::PanelOverlayElement * middlePanel;
        Ogre::PanelOverlayElement * bottomPanel;
};

#endif
