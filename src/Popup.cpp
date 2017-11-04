#include "Popup.h"

#include <string>
#include "OgreOverlayManager.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgrePanelOverlayElement.h"

#include "Id.h"
#include "SnookerApplication.h"
#include "OverlayCommons.h"

using namespace Ogre;

int Popup::order = 10;

Popup::Popup(std::string titleStr, std::string textStr)
{
    SnookerApplication & application = SnookerApplication::getSingleton();
    int lines = 1;
    for (unsigned int i = 0; i < textStr.size(); i++)
        if (textStr[i] == '\n')
            lines++;

    // popup składa się z wierszy tekstu:
    // TITLE
    //
    // CAPTION CAPTION CAPTION
    // CAPTION CAPTION CAPTION
    //
    // PRESS_ENTER
    const double ROW = 20.0; // wysokość wiersza tekstu
    const double HEIGHT = (lines + 4) * ROW; // wysokość popupa

    OverlayManager * manager = OverlayManager::getSingletonPtr();
    overlay = manager->create(Id::generate());
    overlay->setZOrder(order++);
    
    PanelOverlayElement * titlePanel = createPanel("GUI/PopupTitlePanel");
    titlePanel->setLeft((application.getWidth() - titlePanel->getWidth()) / 2);
    titlePanel->setTop((application.getHeight() - HEIGHT) / 2);
    
    PanelOverlayElement * textPanel = createPanel("GUI/PopupTextPanel");
    textPanel->setHeight((lines + 3) * ROW);
    textPanel->setLeft(titlePanel->getLeft());
    textPanel->setTop(titlePanel->getTop() + titlePanel->getHeight());

    TextAreaOverlayElement * title = createTextArea("GUI/PopupText");
    title->setCaption(titleStr);
    titlePanel->addChild(title);
    
    TextAreaOverlayElement * text = createTextArea("GUI/PopupText");
    text->setTop(ROW);
    text->setCaption(textStr);
    textPanel->addChild(text);

    TextAreaOverlayElement * button = createTextArea("GUI/PopupButton");
    button->setTop((lines + 2) * ROW);
    textPanel->addChild(button);

    overlay->add2D(titlePanel);
    overlay->add2D(textPanel);
    overlay->show();
}

Popup::~Popup()
{
    OverlayManager::getSingleton().destroy(overlay);
}
