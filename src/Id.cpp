#include "Id.h"

#include <sstream>

std::string Id::generate(void)
{
    static int id = 0;
    
    std::ostringstream ss;
    ss << "UniqueID_" << ++id;
    return ss.str();
}
