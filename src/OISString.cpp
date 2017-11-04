#include "OISString.h"

#define OIS_DYNAMIC_LIB
#include "OIS/OIS.h"

using namespace OIS;

static char small[256] =
{
    0, 0,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', 0, 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' '
};

static char capital[256] =
{
    0, 0,
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', 0, 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' '
};

OISString::OISString()
{
}

OISString::OISString(const std::string & s)
{
    setString(s);
}

OISString::~OISString()
{
}

void OISString::setString(const std::string & s)
{
    text = s;
}

std::string OISString::getString(void)
{
    return text;
}

bool OISString::injectKey(KeyCode code, bool shift)
{
    if (code > 255)
        return false;

    switch (code) {
        case KC_BACK:
            if (text.size() > 0)
                text.resize(text.size() - 1);
            return true;
        default:
            char c = (shift ? capital[code] : small[code]);
            if (c != 0) {
                text = text + c;
                return true;
            }
            return false;
    }
}
