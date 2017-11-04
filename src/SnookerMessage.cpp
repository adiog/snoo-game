/*
 * SnookerMessage.cpp
 *
 *  Created on: Dec 26, 2008
 *      Author: Karol Ruszczyk
 */

#include "SnookerMessage.h"

namespace network {

const char SnookerMessage::messageDataSeparator = '\n';

SnookerMessage::SnookerMessage() {
	std::memset(data_, 0, header_length + max_data_size);
	data_size_ = -1;

}

SnookerMessage::~SnookerMessage() {
}

SnookerMessage SnookerMessage::recoverMessageFromJunk(const char *message,
		size_t size) {
	SnookerMessage recoveredMessage;

	memcpy(recoveredMessage.data_, message, size);
	recoveredMessage.data_size_ = size;

	return recoveredMessage;
}

SnookerMessage SnookerMessage::recoverMessageFromJunk(std::string message) {
	SnookerMessage recoveredMessage;

	message.copy(recoveredMessage.data_, message.length());
	recoveredMessage.data_size_ = message.length();

	return recoveredMessage;
}

SnookerMessagePtr SnookerMessage::createChatMessage(std::string s) {
	SnookerMessagePtr message(new SnookerMessage());

	const char * str = s.c_str();

	//Kodowanie odpowiedzi
	sprintf(message->data_, "%4d%s", SnookerMessage::chatMessage, str);
	message->data_size_ = strlen(message->data_);

#ifdef DEBUG
	std::cerr << "SnookerMessage::createChatMessage : data to : (" << message->data_ << ") a rozmiar to  : " << message->data_size_
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createLoginRequestMessage(const char* login,
		const char* password) {
	SnookerMessagePtr loginMessage(new SnookerMessage());

	//Kodowanie danych
	sprintf(loginMessage->data_, "%4d%s%c%s", SnookerMessage::loginRequest,
			login, messageDataSeparator, password);
	loginMessage->data_size_ = strlen(loginMessage->data_);

#ifdef DEBUG
	std::cerr << "SnookerMessage::createLoginRequestMessage" << std::endl;
#endif
	return loginMessage;
}

SnookerMessagePtr SnookerMessage::createLoginResponseMessage(int ok) {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::loginResponse);

	message->setInteger(0, ok);

	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createLoginResponseMessage" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createNewGameRequestMessage() {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::newGameRequest);
	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createNewGameRequestMessag" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createNewGameResponseMessage(int newGameId) {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d%4d", SnookerMessage::newGameResponse,
			newGameId);
	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createNewGameResponseMessage" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createGameStartResponseMessage(int ok) {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::gameStartResponse);

	message->setInteger(0, ok);

	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createGameStartResponseMessage" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createGameStartInfoMessage() {
	return createStandardMessage(SnookerMessage::gameStartInfo);
}

SnookerMessagePtr SnookerMessage::createGameListRequestMessage() {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::gameListRequest);
	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createGameListRequestMessage" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createGameListResponseMessage(std::vector<
		std::string> gamesInfo) {
	char *buf;
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::gameListResponse);

	buf = message->data_ + header_length;

	for (unsigned int i = 0; i < gamesInfo.size(); i++) {
		buf += sprintf(buf, "%s\n", gamesInfo[i].c_str());
	}

	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createGameListResponseMessage : data to : ("
	<< message->data_ << ") a rozmiar to  : " << message->data_size_
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createParticipantsListRequestMessage() {
	SnookerMessagePtr message = createStandardMessage(
			SnookerMessage::participantsListRequest);

#ifdef DEBUG
	std::cerr << "SnookerMessage::createParticipantsListRequestMessage"
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createParticipantsListResponseMessage(
		std::vector<std::string> participants) {
	SnookerMessagePtr message = createStandardMessage(
			SnookerMessage::participantsListResponse);

#ifdef DEBUG
	std::cerr << "SnookerMessage::createParticipantsListResponseMessage --> "
	<< participants[0] << ";" << participants[1] << std::endl;
#endif

	message->setSplitedStrings(participants);

#ifdef DEBUG
	std::cerr
	<< "SnookerMessage::createParticipantsListResponseMessage : data to : ("
	<< message->data_ << ") a rozmiar to  : " << message->data_size_
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createJoinGameRequestMessage(int gameId,
		Role role) {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d%4d%4d", SnookerMessage::joinGameRequest,
			gameId, role);
	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createJoinGameRequestMessage : data to : ("
	<< message->data_ << ") a rozmiar to  : " << message->data_size_
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createJoinGameResponseMessage(int ok) {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::joinGameResponse);

	message->setInteger(0, ok);

	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createJoinGameResponseMessage : data to : ("
	<< message->data_ << ") a rozmiar to  : " << message->data_size_
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createLeaveGameRequestMessage() {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::leaveGameRequest);

	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createLeaveGameRequestMessage : data to : ("
	<< message->data_ << ") a rozmiar to  : " << message->data_size_
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createLeaveGameResponseMessage(int success) {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::leaveGameResponse);

	message->setInteger(0, success);

	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createLeaveGameResponseMessage : data to : ("
	<< message->data_ << ") a rozmiar to  : " << message->data_size_
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createGameStartRequestMessage() {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::gameStartRequest);

	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createGameStartRequestMessage" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createPlayerJoinedInfoMessage() {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::playerJoinedInfo);
	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createPlayerJoinedMessage" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createPlayerQuitInfoMessage() {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::playerQuitInfo);
	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createPlayerQuitMessage" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createPlayerLeftInfoMessage() {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", SnookerMessage::playerLeftInfo);
	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createPlayerLeftMessage" << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createSpectatorLeftMessage() {
	return createStandardMessage(SnookerMessage::spectatorLeftInfo);
}

SnookerMessagePtr SnookerMessage::createYourMoveInfoMessage() {
	return createStandardMessage(SnookerMessage::yourMoveInfo);
}

SnookerMessagePtr SnookerMessage::createMoveOfOtherPlayerInfoMessage(
		int playerNumber) {
	SnookerMessagePtr message = createStandardMessage(
			SnookerMessage::moveOfOtherPlayerInfo);
	message->setInteger(0, playerNumber);

	return message;
}

SnookerMessagePtr SnookerMessage::createShotDataInfoMessage(const void * data,
		size_t dataSize) {
	return createBinaryMessage(SnookerMessage::shotDataInfo, data, dataSize);
}

SnookerMessagePtr SnookerMessage::createAfterShotGameStateInfoMessage(
		GameStateInformation gameInfo) {
	std::string gameInfoStr = gameInfo.toString();

#ifdef DEBUG
	std::cerr
	<< "SnookerMessage::createAfterShotGameStateInfoMessage --> przed tworzeniem"
	<< std::endl;
#endif
	SnookerMessagePtr message = createBinaryMessage(
			SnookerMessage::afterShotGameStateInfo, gameInfoStr.c_str(),
			gameInfoStr.length() + 1);
#ifdef DEBUG
	std::cerr
	<< "SnookerMessage::createAfterShotGameStateInfoMessage --> Po tworzeniu"
	<< std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createAfterNonShotGameStateInfoMessage(
		GameStateInformation gameInfo) {
	std::string gameInfoStr = gameInfo.toString();

	SnookerMessagePtr message = createBinaryMessage(
			SnookerMessage::afterNonShotGameStateInfo, gameInfoStr.c_str(),
			gameInfoStr.length() + 1);

	return message;
}

SnookerMessagePtr SnookerMessage::createViewInfoMessage(const void * data,
		size_t dataSize) {
	return createBinaryMessage(SnookerMessage::viewInfo, data, dataSize);
}

SnookerMessagePtr SnookerMessage::createWhiteBallMoveInfoMessage(
		const void * data, size_t dataSize) {
	return createBinaryMessage(SnookerMessage::viewInfo, data, dataSize);
}

SnookerMessagePtr SnookerMessage::createShutUpInfoMessage() {
	return createStandardMessage(SnookerMessage::shutUp);
}

SnookerMessagePtr SnookerMessage::createStandardMessage(
		SnookerMessage::MessageType mt) {
	SnookerMessagePtr message(new SnookerMessage());

	sprintf(message->data_, "%4d", mt);
	message->data_size_ = strlen(message->data_);
#ifdef DEBUG
	std::cerr << "SnookerMessage::createStandardMessage " << mt << std::endl;
#endif

	return message;
}

SnookerMessagePtr SnookerMessage::createBinaryMessage(
		SnookerMessage::MessageType mt, const void * source_data,
		size_t source_data_size) {
	SnookerMessagePtr message(new SnookerMessage());

	//Kodowanie odpowiedzi
	sprintf(message->data_, "%4d", mt);
	std::memcpy(message->data_ + header_length, source_data, source_data_size);

	message->data_size_ = source_data_size + header_length;
#ifdef DEBUG
	std::cerr
	<< "SnookerMessage::createBinaryMessage : data to : (binarne dane wiec nie wyswietlam) a rozmiar to (razem z naglowkiem)  : "
	<< message->data_size_ << std::endl;
#endif
	return message;
}

std::vector<std::string> SnookerMessage::getLoginRequestDetails() {
	return this->getSplitedStrings();
}

bool SnookerMessage::getLoginConfirmation() {
	return this->getInteger(0);
}

bool SnookerMessage::getJoinGameConfirmation() {
	return this->getInteger(0);
}

int SnookerMessage::getNewGameNumber() {
	return this->getInteger(0);
}

GameStateInformation SnookerMessage::getGameStateInformation() {
	GameStateInformation gameInfo(std::string(data_ + header_length));
	return gameInfo;
}

std::vector<std::string> SnookerMessage::getGamesList() {
	return this->getSplitedStrings();
}

std::string SnookerMessage::getPlayer1() {
	std::vector<std::string> all = this->getSplitedStrings();
	std::string result = "";
	if (all[0] != "???") {
		result = all[0];
	}
	return result;
}

std::string SnookerMessage::getPlayer2() {
	std::vector<std::string> all = this->getSplitedStrings();
	std::string result = "";
	if (all[0] != "???") {
		result = all[1];
	}
	return result;
}

std::vector<std::string> SnookerMessage::getParticipiants() {
	std::vector<std::string> all = this->getSplitedStrings();
	return all;
}

int SnookerMessage::getGameNumberToJoin() {
	return this->getInteger(0);
}

SnookerMessage::Role SnookerMessage::getRoleToJoin() {
	return (Role) this->getInteger(1);
}

std::string SnookerMessage::getChatMessage() {
	std::string s(data_ + header_length);

	return s;
}

size_t SnookerMessage::getBinaryData(void * data, size_t size) {
	int x = (size < data_size_ - header_length) ? size : (data_size_
			- header_length);
	std::memcpy(data, data_ + header_length, x);
	return x;
}

const void * SnookerMessage::getBinaryDataPointer() {
	return data_ + header_length;
}

SnookerMessage::MessageType SnookerMessage::getType() {
	int temp;
	char buf[header_length + 1];
	MessageType messageType;

	memset(buf, 0, header_length + 1);

	memcpy(buf, data_, header_length);
	buf[header_length] = '\0';

	if (sscanf(buf, "%d", &temp)) {
		messageType = (MessageType) temp;
	} else {
#ifdef DEBUG
		std::cerr << "SnookerMessage::getType --> Nie udalo sie przeczytac.."
		<< std::endl;
#endif
		assert(false);
	}
	return messageType;
}

size_t SnookerMessage::get_data_size() const {
	return data_size_;
}
char* SnookerMessage::get_data() {
	return data_;
}

//Zwraca n-tego (liczac od 0) int'a zakodowanego w wiadomosci
int SnookerMessage::getInteger(int n) {
	char buf[default_int_length + 1];
	int value = -1;

	memcpy(buf, data_ + header_length + (default_int_length * n),
			default_int_length);
	buf[default_int_length] = '\0';

	sscanf(buf, "%d", &value);
	return value;
}

void SnookerMessage::setInteger(int pos, int value) {
	sprintf(data_ + header_length + (pos * default_int_length), "%4d", value);
}

void SnookerMessage::setSplitedStrings(std::vector<std::string> strings) {
	char *buf = data_ + header_length;

	for (unsigned int i = 0; i < strings.size(); i++) {
		buf += sprintf(buf, "%s%c", strings[i].c_str(), messageDataSeparator);
	}

	data_size_ = strlen(data_);
}

std::vector<std::string> SnookerMessage::getSplitedStrings() {
	std::vector<std::string> result;
	std::string delim(1, messageDataSeparator);
	std::string bufor(data_ + header_length);

	Utils::splitStr(result, bufor, delim);

	return result;
}

}
