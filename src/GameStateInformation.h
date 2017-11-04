/*
 * GameStateInformation.h
 *
 *  Created on: Mar 26, 2009
 *      Author: karol
 */

#ifndef GAMESTATEINFORMATION_H_
#define GAMESTATEINFORMATION_H_

#include <string>
#include <sstream>

namespace network {

class GameStateInformation {
public:

	enum {
		none = 7,
		breakFinished,
		gameFinished,
		resign
	};

	int pointsAfter[2];
	int pointsBefore[2];
	int breakScore;
	int extraAction;
	int points;
	std::string ballsBefore;
	std::string ballsAfter;
	std::string shotParams;
	std::string description;
	std::string stateAfter;

	GameStateInformation();
	GameStateInformation(std::string);

	std::string toString();

};

}

#endif /* GAMESTATEINFORMATION_H_ */
