/*
 * SnookerClient.cpp
 *
 *  Created on: Dec 27, 2008
 *      Author: Karol Ruszczyk
 */

#include "SnookerClient.h"

namespace network {

SnookerClient::SnookerClient() {
}

SnookerClient::~SnookerClient() {
}

void SnookerClient::establishConnection(const char *server, const char *port) {
	if (network_client_.get() != NULL) {
		this->disconnect();
	}

	NetworkClientPtr new_network_client(new NetworkClient());

	new_network_client->establishConnection(server, port);

	this->network_client_ = new_network_client;
}

bool SnookerClient::isConnecting() {
	if (network_client_.get() != NULL) {
		return this->network_client_->isConnecting();
	}
	return false;
}

bool SnookerClient::isConnected() {
	if (network_client_.get() != NULL) {
		return this->network_client_->isConnected();
	}
	return false;
}

void SnookerClient::disconnect() {
	network_client_->disconnect();
	network_client_.reset();
}

int SnookerClient::messagesWaiting() {
	return network_client_->messagesWaiting();
}

void SnookerClient::sendLoginRequest(const char* login, const char* password) {
	SnookerMessagePtr loginMessage = SnookerMessage::createLoginRequestMessage(login, password);
	network_client_->send(loginMessage->get_data_size(), loginMessage->get_data());
}

void SnookerClient::sendCreateAndJoinNewGameRequest() {
	SnookerMessagePtr message = SnookerMessage::createNewGameRequestMessage();
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendGamesListRequest() {
	SnookerMessagePtr message = SnookerMessage::createGameListRequestMessage();
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendParticipantsListRequest() {
	SnookerMessagePtr message = SnookerMessage::createParticipantsListRequestMessage();
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendGameStartRequest() {
	SnookerMessagePtr message = SnookerMessage::createGameStartRequestMessage();
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendJoinGameAsPlayerRequest(int gameId) {
	SnookerMessagePtr message =
			SnookerMessage::createJoinGameRequestMessage(gameId, SnookerMessage::player);
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendJoinGameAsSpectatorRequest(int gameId) {
	SnookerMessagePtr message =
			SnookerMessage::createJoinGameRequestMessage(gameId, SnookerMessage::spectator);
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendLeaveGameRequest() {
	SnookerMessagePtr message = SnookerMessage::createLeaveGameRequestMessage();
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendChatMessage(std::string s) {
	SnookerMessagePtr message = SnookerMessage::createChatMessage(s);
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendShotDataInfo(const void * data, size_t dataSize) {
	SnookerMessagePtr message = SnookerMessage::createShotDataInfoMessage(data, dataSize);
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendAfterShotGameStateInfo(GameStateInformation gameInfo) {
	std::cerr << "SnookerClient::sendAfterShotGameStateInfo --> Przed wyslaniem " << std::endl;
        std::cerr << "extraAction = " << gameInfo.extraAction << std::endl;

	SnookerMessagePtr message = SnookerMessage::createAfterShotGameStateInfoMessage(gameInfo);
        std::cerr << "extraAction = " << gameInfo.extraAction << std::endl;

	network_client_->send(message->get_data_size(), message->get_data());
	std::cerr << "SnookerClient::sendAfterShotGameStateInfo --> Po wyslaniu " << std::endl;
}

void SnookerClient::sendAfterNonShotGameStateInfo(GameStateInformation gameInfo) {
	SnookerMessagePtr message = SnookerMessage::createAfterNonShotGameStateInfoMessage(gameInfo);

	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendViewInfo(const void * data, size_t dataSize) {
	SnookerMessagePtr message = SnookerMessage::createViewInfoMessage(data, dataSize);
	network_client_->send(message->get_data_size(), message->get_data());
}

void SnookerClient::sendWhiteBallMoveInfo(const void * data, size_t dataSize) {
	SnookerMessagePtr message = SnookerMessage::createWhiteBallMoveInfoMessage(data, dataSize);
	network_client_->send(message->get_data_size(), message->get_data());
}

SnookerMessage SnookerClient::getMessage() {
	std::string messageString;
	network_client_->getMessage(messageString); // TODO Thread safe??

	return SnookerMessage::recoverMessageFromJunk(messageString);
}

void SnookerClient::removeAllWaitingMessages() {
	std::string s;
	while (network_client_->getMessage(s))
		;
}

}
