/*
 * Server.cpp
 *
 *  Created on: Nov 28, 2008
 *      Author: Karol Ruszczyk
 */

#include "NetworkServer.h"

NetworkServer::NetworkServer() :
	next_id_(1) {
}

NetworkServer::~NetworkServer() {

}

bool NetworkServer::start(unsigned short port) {
	try {
		endpoint_ = new boost::asio::ip::tcp::endpoint(tcp::v4(), port);
		acceptor_ = new boost::asio::ip::tcp::acceptor(io_service_, *endpoint_);

		NetworkSessionPtr newSession(new NetworkSession(io_service_, (*this), next_id_));
		acceptor_->async_accept(newSession->socket(), boost::bind(&NetworkServer::handleNewConnection, this,
				newSession, boost::asio::placeholders::error));

		thread_.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
	} catch (std::exception& e) {
		return false;
	}

	return true;
}

void NetworkServer::handleNewConnection(NetworkSessionPtr session, const boost::system::error_code& error) {
	if (!error) {
		activeSessions_.insert(std::make_pair(next_id_++, session));
		session->start();
		this->onClientConnect(next_id_ - 1);
		NetworkSessionPtr new_session(new NetworkSession(io_service_, (*this), next_id_));
		acceptor_->async_accept(new_session->socket(), boost::bind(&NetworkServer::handleNewConnection, this,
				new_session, boost::asio::placeholders::error));
	}
}

void NetworkServer::disconnect(int clientId) {
	onClientDisconnect(clientId);
	activeSessions_.erase(clientId);
}

bool NetworkServer::deliver(int destinationId, size_t size, const char *data) const {
	NetworkPackage msg(size, data);
	this->deliver(destinationId, msg);
	return true;
}

void NetworkServer::deliver(int destinationId, NetworkPackage msg) const {

	std::map<int, NetworkSessionPtr>::const_iterator iter = activeSessions_.find(destinationId);
	if (iter != activeSessions_.end()) {
		(iter->second)->deliver(boost::ref(msg));
	}

}

bool NetworkServer::stop() {
	io_service_.stop(); // TODO nie jestem pewien czy to robi to co powinno, ale raczej tak
	thread_->join();
	return true;
}

