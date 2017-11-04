#ifndef __SnookerEventListener_h__
#define __SnookerEventListener_h__

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

class SnookerEventListener : public OIS::KeyListener, public OIS::MouseListener {
    public:
        SnookerEventListener();
        virtual ~SnookerEventListener();
        bool keyPressed(const OIS::KeyEvent &event);
        bool keyReleased(const OIS::KeyEvent &event);
        bool mouseMoved(const OIS::MouseEvent &event);
        bool mousePressed(const OIS::MouseEvent &event, OIS::MouseButtonID id);
        bool mouseReleased(const OIS::MouseEvent &event, OIS::MouseButtonID id);
};

#endif
