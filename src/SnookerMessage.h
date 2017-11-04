/*
 * SnookerMessage.h
 *
 *  Created on: Dec 26, 2008
 *      Author: Karol Ruszczyk
 */

#ifndef SNOOKERMESSAGE_H_
#define SNOOKERMESSAGE_H_

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

#include "GameStateInformation.h"
#include "NetworkPackage.h"
#include "Utils.h"

namespace network {

struct GameInformation {
	int id;
	std::string player1;
	std::string player2;
};

class SnookerMessage;

typedef boost::shared_ptr<std::string> StringPtr;
typedef boost::shared_ptr<SnookerMessage> SnookerMessagePtr;

class SnookerMessage {
public:

	static const char messageDataSeparator;

	enum {
		max_data_size = NetworkPackage::max_input_data_size
	};

	enum {
		header_length = 4
	};

	enum {
		default_int_length = 4
	};

	enum MessageType {
		shutUp = 0,
		loginRequest,
		loginResponse,
		newGameRequest,
		newGameResponse,
		gameListRequest,			//5
		gameListResponse,
		participantsListRequest,
		participantsListResponse,
		joinGameRequest,
		joinGameResponse,			//10
		leaveGameRequest,
		leaveGameResponse,
		gameStartResponse,
		gameStartRequest,
		gameStartInfo,				//15
		playerJoinedInfo,
		playerQuitInfo,
		playerLeftInfo,
		spectatorLeftInfo,
		yourMoveInfo,				//20
		moveOfOtherPlayerInfo,
		chatMessage,
		shotDataInfo,
		afterShotGameStateInfo,
		afterNonShotGameStateInfo,  //25
		viewInfo,
		whiteBallMoveInfo
		};

	enum Role {
		player = 0,
		spectator
	};

	//Constructors
	static SnookerMessagePtr createChatMessage(std::string s);

    static SnookerMessagePtr createLoginRequestMessage(const char *login, const char *password);
    static SnookerMessagePtr createLoginResponseMessage(int ok);
    static SnookerMessagePtr createNewGameRequestMessage();
    static SnookerMessagePtr createNewGameResponseMessage(int newGameId);
    static SnookerMessagePtr createGameStartRequestMessage();
    static SnookerMessagePtr createGameStartResponseMessage(int ok);
    static SnookerMessagePtr createGameStartInfoMessage();
    static SnookerMessagePtr createGameListRequestMessage();
    static SnookerMessagePtr createGameListResponseMessage(std::vector<std::string> gamesInfo);
    static SnookerMessagePtr createParticipantsListRequestMessage();
    static SnookerMessagePtr createParticipantsListResponseMessage(std::vector<std::string> participants);
    static SnookerMessagePtr createJoinGameRequestMessage(int gameId, Role role);
    static SnookerMessagePtr createJoinGameResponseMessage(int ok);
    static SnookerMessagePtr createLeaveGameRequestMessage();
    static SnookerMessagePtr createLeaveGameResponseMessage(int success);
    static SnookerMessagePtr createPlayerJoinedInfoMessage();
    static SnookerMessagePtr createPlayerQuitInfoMessage();
    static SnookerMessagePtr createPlayerLeftInfoMessage();
    static SnookerMessagePtr createSpectatorLeftMessage();
    static SnookerMessagePtr createYourMoveInfoMessage();
    static SnookerMessagePtr createMoveOfOtherPlayerInfoMessage(int playerNumber);
    static SnookerMessagePtr createShotDataInfoMessage(const void * data, size_t dataSize);
    static SnookerMessagePtr createAfterShotGameStateInfoMessage(GameStateInformation gameStateInformation);
    static SnookerMessagePtr createAfterNonShotGameStateInfoMessage(GameStateInformation gameStateInformation);
    static SnookerMessagePtr createViewInfoMessage(const void * data, size_t dataSize);
    static SnookerMessagePtr createWhiteBallMoveInfoMessage(const void * data, size_t dataSize);
    static SnookerMessagePtr createShutUpInfoMessage();
    static SnookerMessage recoverMessageFromJunk(const char *message, size_t size);
    static SnookerMessage recoverMessageFromJunk(std::string message);

    virtual ~SnookerMessage();


    Role getRoleToJoin();
    bool getLoginConfirmation();
    bool getJoinGameConfirmation();
    int getGameNumberToJoin();
    int getNewGameNumber();
    std::string getChatMessage();
    std::vector<std::string> getLoginRequestDetails();
    std::vector<std::string> getGamesList();

    GameStateInformation getGameStateInformation();

    //participantsListResponse
    std::string getPlayer1();
    std::string getPlayer2();
    std::vector<std::string> getParticipiants();

    size_t getBinaryData(void * data, size_t size);
    const void * getBinaryDataPointer();

    virtual MessageType getType();

    size_t get_data_size() const;

    char* get_data();

private:
	size_t data_size_;
	char data_[header_length + max_data_size];

	SnookerMessage();
	static SnookerMessagePtr createStandardMessage(SnookerMessage::MessageType mt);
	static SnookerMessagePtr createBinaryMessage(SnookerMessage::MessageType mt, const void * data, size_t data_size);

	int getInteger(int pos);
	void setInteger(int pos, int value);

	void setSplitedStrings(std::vector<std::string> strings);
	std::vector<std::string> getSplitedStrings();

};

}

#endif /* SNOOKERMESSAGE_H_ */
