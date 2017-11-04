#ifndef DBASE_H_
#define DBASE_H_

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "DBaseParams.h"
#include "GameStateInformation.h"


int test();

bool DBaseConnect();
void DBaseDisconnect();
int DBaseInsert(std::string query);
int DBaseAuthenticate(std::string user, std::string pass);
int DBaseGetShotsForReplay(int id, int * id_array, int maxnum);
void DBaseGetShot(int, network::GameStateInformation **);
void DBaseGetLastShot(network::GameStateInformation **);
#endif /* DBASE_H_ */

