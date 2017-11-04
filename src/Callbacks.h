#ifndef __Callbacks_h__
#define __Callbacks_h__

#include "MenuItem.h"

bool cb_closeApplication(MenuItem *mi);
bool cb_newGame(MenuItem * mi);
bool cb_connect(MenuItem * mi);
bool cb_cueConnect(MenuItem * mi);
bool cb_training(MenuItem * mi);
bool cb_help(MenuItem * mi);
bool cb_wireframe(MenuItem * mi);

#endif
