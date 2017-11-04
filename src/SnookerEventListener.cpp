#include <iostream>

#include "SnookerEventListener.h"
#include "SnookerApplication.h"
#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

using namespace OIS;

SnookerEventListener::SnookerEventListener()
{
}

SnookerEventListener::~SnookerEventListener()
{
}

bool SnookerEventListener::keyPressed(const KeyEvent &event)
{
    return true;
}

bool SnookerEventListener::keyReleased(const KeyEvent &event)
{
    SnookerApplication & application = SnookerApplication::getSingleton();
 
    // Sprawdźmy, czy gui nie łyknie naszego klawisza
    if (application.gui.injectKey(event.key))
        return true;
    
    switch (event.key) {
        case KC_ESCAPE:
            application.gui.openMainMenu();
            break;
        case KC_F1:
            application.setCameraMode(SnookerApplication::CAMERA_MODE_DYNAMIC);
            break;
        case KC_F2:
            application.setCameraMode(SnookerApplication::CAMERA_MODE_TV);
            break;
        case KC_F3:
            application.setCameraMode(SnookerApplication::CAMERA_MODE_TOP);
            break;
        case KC_F5:
            application.initReplay(1000.0);
            break;
        case KC_F8:
            application.screenshot();
            break;
        case KC_F9:
            application.initReplay(12.0);
            break;
        case KC_F10:
            application.initReplay(6.0);
            break;
        case KC_F11:
            application.initReplay(3.0);
            break;
        case KC_F12:
            application.initReplay(1.0);
            break;
        default:
            break;
    }
    return true;
}

bool SnookerEventListener::mouseMoved(const MouseEvent &event)
{
    SnookerApplication & application = SnookerApplication::getSingleton();
    Keyboard * keyboard = application.getKeyboard();
    bool shiftPressed = keyboard->isKeyDown(KC_LSHIFT) || keyboard->isKeyDown(KC_RSHIFT);
    bool ctrlPressed = keyboard->isKeyDown(KC_LCONTROL) || keyboard->isKeyDown(KC_RCONTROL);
    bool tabPressed = keyboard->isKeyDown(KC_TAB);

    double dx = event.state.X.rel;
    double dy = event.state.Y.rel;
    double dz = event.state.Z.rel;

    if (dz > 0.5)
        application.gui.injectKey(OIS::KC_UP);
    if (dz < -0.5)
        application.gui.injectKey(OIS::KC_DOWN);

    if (shiftPressed) {
        // zmiana punktu na białej bili
        application.moveWhiteBallPoint(dx / 500.0, dy / 500.0);
    } else if (tabPressed) {
        // zmiana polozenia bialej bili
        application.moveWhiteBall(dx / 70.0, dy / 70.0);
    } else {
        // po prostu zmiana kierunku widzenia / kata uderzania
        double div = (ctrlPressed ? 30000.0 : 1000.0);
        if (application.canLookAround()) {
            application.rotateDirection(-dx / div);
            application.rotateTilt(dy / div);
        }
    }

    return true;
}

bool SnookerEventListener::mousePressed(const MouseEvent &event, MouseButtonID id)
{
    SnookerApplication::getSingleton().setButtonPressed();
    return true;
}

bool SnookerEventListener::mouseReleased(const MouseEvent &event, MouseButtonID id)
{
    SnookerApplication::getSingleton().releaseButton();
    return true;
}
