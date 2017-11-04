#include "Runtime.h"

#include <string>
#include <cstdlib>
#include <iostream>


std::string Runtime::workDir(void)
{
    std::string workdir = "./";
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    char * homeenv = getenv("HOME");
    if (homeenv == NULL) {
        std::cerr << "$HOME not set" << std::endl;
    } else
        workdir = std::string(homeenv) + "/.snoo/";
#endif
    return workdir;
}
