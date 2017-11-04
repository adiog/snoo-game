#ifndef __SnookerSettings_h__
#define __SnookerSettings_h__

#include <string>

class SnookerSettings {
    public:
        std::string gameServer;
        std::string gamePort;
        std::string cueServer;
        std::string cuePort;
        std::string login;
        std::string password;
        SnookerSettings();
        virtual ~SnookerSettings();
        void save(void); ///< Zapisuje ustawienia do pliku
        void load(void); ///< Odczytuje ustawienia z pliku
};

#endif
