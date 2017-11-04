/*
 * GameStateInformation.cpp
 *
 *  Created on: Mar 26, 2009
 *      Author: karol
 */

#include "GameStateInformation.h"
#include <iostream>

namespace network {

GameStateInformation::GameStateInformation()
{
}

GameStateInformation::GameStateInformation(std::string inputData)
{
	std::istringstream sstream(inputData);
        sstream >> pointsAfter[0]
			>> pointsAfter[1]
			>> pointsBefore[0]
			>> pointsBefore[1]
			>> breakScore
			>> extraAction
			>> points;

	getline(sstream, ballsBefore);
	getline(sstream, ballsBefore);
	getline(sstream, ballsAfter);
	getline(sstream, shotParams);
	getline(sstream, description);
	getline(sstream, stateAfter);

}

std::string GameStateInformation::toString()
{
	std::ostringstream sstream;
	sstream << pointsAfter[0] << "\n"
			<< pointsAfter[1] << "\n"
			<< pointsBefore[0] << "\n"
			<< pointsBefore[1] << "\n"
			<< breakScore << "\n"
			<< extraAction << "\n"
			<< points << "\n"
			<< ballsBefore << "\n"
			<< ballsAfter << "\n"
			<< shotParams << "\n"
			<< description << "\n"
			<< stateAfter;

	return sstream.str();
}

}
