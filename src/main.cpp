#include <iostream>

#include "SnookerApplication.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 
#define WIN32_LEAN_AND_MEAN 
#include "windows.h" 
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT) 
{
#else 
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
int main(int argc, char **argv) 
{
    char * home = getenv("HOME");
    // Pobieramy sciezke do katalogu domowego
    if (home == NULL) {
        std::cerr << "$HOME not set" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Tworzymy (jesli go nie ma) katalog ~/.snoo
    std::string workdir = std::string(home) + "/.snoo/";
    int err = mkdir(workdir.c_str(), 0700);
    if (err != 0 && errno != EEXIST) {
        perror("Cannot create directory ~/.snoo/");
        exit(EXIT_FAILURE);
    }

    // Sprawdzamy, czy ~/.snoo jest katalogiem
    DIR * dir = opendir(workdir.c_str());
    if (dir == NULL) {
        perror("Cannot access directory ~/.snoo/");
        exit(EXIT_FAILURE);
    }
    if (closedir(dir) != 0) {
        perror("Close ~/.snoo/");
        exit(EXIT_FAILURE);
    }

    // Zmieniamy katalog roboczy na katalog DATA_DIR
#define DATA_DIR "."
    if (chdir(DATA_DIR) != 0) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }
#endif 
    SnookerApplication & app = SnookerApplication::getSingleton();
    app.go();
    return 0;
}
