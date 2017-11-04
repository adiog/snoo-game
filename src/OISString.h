#ifndef __OISString_h__
#define __OISString_h__

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

class OISString {
    public:
        OISString();
        OISString(const std::string &);
        virtual ~OISString();
        virtual void setString(const std::string &);
        virtual std::string getString(void);

        /// Zmienia przechowywany napis wg. naciśniętego klawisza i zwraca true.
        /// Jeśli klawisz nie ma zastosowania - zwraca false.
        virtual bool injectKey(OIS::KeyCode, bool);
    private:
        std::string text;
};

#endif
