#include "Callbacks.h"

#include <vector>
#include "SnookerApplication.h"
#include "Menu.h"
#include "MenuItem.h"
#include "MenuItemCommand.h"
#include "NetworkClient.h"

using namespace std;

bool cb_closeApplication(MenuItem * mi)
{
    SnookerApplication::getSingleton().close();
    return true;
}

bool cb_newGame(MenuItem * mi)
{
    SnookerApplication & app = SnookerApplication::getSingleton();
    
    app.startMatch();
    return true;
}

bool cb_connect(MenuItem * mi)
{
    SnookerApplication::getSingleton().connect();
    return true;
}

bool cb_cueConnect(MenuItem * mi)
{
    SnookerApplication & app = SnookerApplication::getSingleton();
    app.cueConnect();
    return true;
}

bool cb_training(MenuItem * mi)
{
    SnookerApplication & app = SnookerApplication::getSingleton();
    app.startTraining();
    return true;
}


bool cb_help(MenuItem * mi)
{
    SnookerApplication::getSingleton().gui.closeMenu();
    SnookerApplication::getSingleton().gui.showMessage(
            "Help",
            "Move your mouse to aim.\n"
            "Hold Ctrl to aim more accurately.\n"
            "Hold Shift and move your mouse to choose the\n"
            "hit point on the cue ball.\n"
            "Hold Tab and move your mouse to choose position\n"
            "for the cue ball (only when the cue ball was potted\n"
            "and before the first shot).\n"
            "Press the mouse button to hit the ball.\n"
            "(hit force is related to button pressed time)\n\n"
            "F1 - normal view        \n"
            "F2 - TV view           \n"
            "F3 - 2D view           \n\n"
            "F9  - Replay, speed 8%  \n"
            "F10 - Replay, speed 16% \n"
            "F11 - Replay, speed 33% \n"
            "F12 - Replay, speed 100%\n"
            );

    return true;
}

bool cb_wireframe(MenuItem * mi)
{
    SnookerApplication::getSingleton().scene->toggleWireframe();
    return true;
}
