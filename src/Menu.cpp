#include "Menu.h"

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"
#include "MenuItem.h"
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgrePanelOverlayElement.h>
#include <OGRE/Overlay/OgreOverlayManager.h>

using namespace Ogre;
using namespace OIS;

Menu::Menu(std::string name):
    parentsChild(NULL),
    subMenu(NULL),
    position(0),
    selected(false),
    width(350),
    height(0),
    visible(false)
{
    OverlayManager * manager = OverlayManager::getSingletonPtr();
    overlay = manager->create(name);
    topPanel = dynamic_cast<PanelOverlayElement*> (manager->
            createOverlayElementFromTemplate("Snoo/MenuBoxTop", "Panel", name + "Top"));
    middlePanel = dynamic_cast<PanelOverlayElement*> (manager->
            createOverlayElementFromTemplate("Snoo/MenuBoxMiddle", "Panel", name + "Middle"));
    bottomPanel = dynamic_cast<PanelOverlayElement*> (manager->
            createOverlayElementFromTemplate("Snoo/MenuBoxBottom", "Panel", name + "Bottom"));
    overlay->add2D(topPanel);
    overlay->add2D(middlePanel);
    overlay->add2D(bottomPanel);
    setWidth(width);
}

Menu::~Menu()
{
    while (! items.empty()) {
        delete items[items.size() - 1];
        items.pop_back();
    }
}

MenuItem * Menu::addMenuItem(MenuItem *item)
{
    items.push_back(item);
    item->setParentMenu(this);
    item->setTop(height);
    item->setWidth(middlePanel->getWidth() - 50);
    middlePanel->addChild(item->getOverlayElement());

    height += item->getHeight(); 
    bottomPanel->setTop(top + height + 25);
    middlePanel->setHeight(height);

    return item;
}

void Menu::injectKey(KeyCode code)
{

    if (subMenu != NULL) {
        subMenu->injectKey(code);
    }  else {
        bool canExit = true;
        
        switch (code) {
            case KC_ESCAPE:
            case KC_LEFT:
                for (unsigned int i = 0; i < items.size(); ++i)
                    if (! (items[i]->onClose(items[i])))
                        canExit = false;
                if (canExit) {
                    if (selected)
                        items[position]->deselect();
                    selected = false;
                    *parentsChild = NULL;
                    overlay->hide();
                }
                break;
            case KC_UP:
                if (selected)
                    items[position]->deselect();
                if (!hasSelectableItem())
                    break;
                do {
                    position--;
                    if (position < 0)
                        position = items.size() - 1;
                } while (!items[position]->isSelectable());
                items[position]->select();
                selected = true;
                break;
            case KC_DOWN:
                if (selected)
                    items[position]->deselect();
                if (!hasSelectableItem())
                    break;
                do {
                    position = (position + 1) % items.size();
                } while (!items[position]->isSelectable());
                items[position]->select();
                selected = true;
                break;
            default:
                if (selected)
                    items[position]->injectKey(code);
                break;
        }
    }
}

int Menu::getHeight(void)
{
    return height;
}

void Menu::setHeight(int h)
{
    return;
}

int Menu::getWidth(void)
{
    return width;
}

void Menu::setWidth(int w)
{
    topPanel->setWidth(w);
    middlePanel->setWidth(w);
    bottomPanel->setWidth(w);
    for (unsigned int i = 0; i < items.size(); i++)
        items[i]->setWidth(w);
}

int Menu::getTop(void)
{
    return top;
}

void Menu::setTop(int t)
{
    top = t;
    topPanel->setTop(top);
    middlePanel->setTop(top + 25);
    bottomPanel->setTop(top + 25 + height);
}

int Menu::getLeft(void)
{
    return left;
}

void Menu::setLeft(int l)
{
    left = l;
    topPanel->setLeft(left);
    middlePanel->setLeft(left);
    bottomPanel->setLeft(left);
}

bool Menu::getVisible(void)
{
    return visible;
}

void Menu::setVisible(bool v)
{
    visible = v;
    if (visible)
        overlay->show();
    else
        overlay->hide();
}

void Menu::open(Menu ** pc)
{
    parentsChild = pc;
    *pc = this;
    setVisible(true);
    if (hasSelectableItem()) {
        position = items.size() - 1;
        injectKey(KC_DOWN);
    }
}

void Menu::close(void)
{
    if (subMenu != NULL)
        subMenu->close();
    if (selected)
        items[position]->deselect();
    setVisible(false);
    *parentsChild = NULL;
}

bool Menu::hasSelectableItem()
{
    bool has = false;
    for (unsigned int i = 0; i < items.size(); i++)
        if (items[i]->isSelectable())
            has = true;
    return has;
}
