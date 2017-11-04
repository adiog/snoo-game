/*
 * Server.h
 *
 *  Created on: Nov 28, 2008
 *      Author: Karol Ruszczyk
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <deque>
#include <map>

#include "NetworkPackage.h"
#include "NetworkSession.h"

class NetworkSession;

typedef boost::shared_ptr<NetworkSession> NetworkSessionPtr;

typedef boost::shared_ptr<boost::thread> ThreadPtr;

class NetworkServer {
public:
	NetworkServer();
	~NetworkServer();

	virtual bool start(unsigned short port);
	virtual void onClientConnect(int id) = 0;
	virtual void onClientDisconnect(int id) = 0;
	virtual void onDataReceived(int sender, NetworkPackage msg) = 0;
	virtual bool deliver(int destinationId, size_t size, const char *data) const;
	virtual void deliver(int destinationId, NetworkPackage msg) const;

	virtual void disconnect(int clientId);

	virtual bool stop(); //wyłacza serwer

	//virtual bool sendString(const int id, const char *data);
	/* jeszcze jakieś funkcje do zarządzania klientami typu
	 * diconnect(int id);
	 */
private:
	virtual void handleNewConnection(NetworkSessionPtr session,
			const boost::system::error_code& error);

	std::map<int, NetworkSessionPtr> activeSessions_;

	boost::asio::io_service io_service_;

	boost::asio::ip::tcp::acceptor* acceptor_;
	boost::asio::ip::tcp::endpoint* endpoint_;
	ThreadPtr thread_;

	int next_id_;
};

#endif /* SERVER_H_ */
