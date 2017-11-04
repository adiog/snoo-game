#include "SnookerSettings.h"

#include <iostream>
#include <fstream>
#include "Runtime.h"

const int BUFFER_SIZE = 1024;

using namespace std;

SnookerSettings::SnookerSettings()
{
    load();
}

SnookerSettings::~SnookerSettings()
{
    save();
}

void SnookerSettings::save(void)
{
    std::string filename = Runtime::workDir() + "snoo.conf";
    ofstream file(filename.c_str(), ios::out);
    file
        << gameServer << "\n"
        << gamePort << "\n"
        << cueServer << "\n"
        << cuePort << "\n"
        << login << "\n"
        << password << "\n";
    file.close();
}

void SnookerSettings::load(void)
{
    char buffer[BUFFER_SIZE];
    std::string filename = Runtime::workDir() + "snoo.conf";
    ifstream file(filename.c_str(), ios::in);

    file.getline(buffer, BUFFER_SIZE);
    gameServer = string(buffer);
    file.getline(buffer, BUFFER_SIZE);
    gamePort = string(buffer);
    file.getline(buffer, BUFFER_SIZE);
    cueServer = string(buffer);
    file.getline(buffer, BUFFER_SIZE);
    cuePort = string(buffer);
    file.getline(buffer, BUFFER_SIZE);
    login = string(buffer);
    file.getline(buffer, BUFFER_SIZE);
    password = string(buffer);
    file.close();
}
