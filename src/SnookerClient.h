/*
 * SnookerClient.h
 *
 *  Created on: Dec 27, 2008
 *      Author: Karol Ruszczyk
 */

#ifndef SNOOKERCLIENT_H_
#define SNOOKERCLIENT_H_

//exceptions
#define WRONG_MESSAGE_EXCEPTION 50
#define SEND_MESSAGE_ERROR_EXCEPTION 51


#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

#include "NetworkClient.h"
#include "SnookerMessage.h"

namespace network {

typedef boost::shared_ptr<NetworkClient> NetworkClientPtr;

class SnookerClient {
public:
	SnookerClient();
	virtual ~SnookerClient();

	virtual void establishConnection(const char *server, const char *port);
	virtual bool isConnecting();
	virtual bool isConnected();
	virtual void disconnect();
	virtual int messagesWaiting();


	virtual void sendLoginRequest(const char* login, const char* password);

	//Tworzy nowy "pokoj" i automatycznie do niego dolacza
	virtual void sendCreateAndJoinNewGameRequest();
	virtual void sendGamesListRequest();
	virtual void sendParticipantsListRequest();
	virtual void sendGameStartRequest();
	virtual void sendJoinGameAsPlayerRequest(int gameId);
	virtual void sendJoinGameAsSpectatorRequest(int gameId);
	virtual void sendLeaveGameRequest();
	virtual void sendChatMessage(std::string s);
	virtual void sendShotDataInfo(const void * data, size_t dataSize);
	virtual void sendAfterShotGameStateInfo(GameStateInformation gameInfo);
	virtual void sendAfterNonShotGameStateInfo(GameStateInformation gameInfo);
	virtual void sendViewInfo(const void * data, size_t dataSize);
	virtual void sendWhiteBallMoveInfo(const void * data, size_t dataSize);

	virtual SnookerMessage getMessage();

	virtual void removeAllWaitingMessages();

	//pewnie tu bedzie cos jeszcze do przesylania pozycji bil itp

private:
	NetworkClientPtr network_client_;
};

}

#endif /* SNOOKERCLIENT_H_ */
