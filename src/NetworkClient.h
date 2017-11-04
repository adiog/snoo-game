/*
 * NetworkClient.h
 *
 *  Created on: Nov 15, 2008
 *      Author: Karol Ruszczyk
 */

#ifndef NETWORKCLIENT_H_
#define NETWORKCLIENT_H_

#include <cstdlib>
#include <deque>
#include <iostream>
#include <sstream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include "NetworkPackage.h"

typedef boost::shared_ptr<NetworkPackage> NetworkPackagePtr;

typedef std::deque<NetworkPackagePtr> NetworkPackagePtrQueue;

using boost::asio::ip::tcp;

class NetworkClient {
public:
	NetworkClient(int id);
	NetworkClient();
	virtual ~NetworkClient();

	virtual void establishConnection(const char *server, const char *port);
	virtual int messagesWaiting();
	virtual void onMessageReceived();
	virtual bool send(size_t size, const void * data);
	virtual void disconnect();

	virtual bool isConnected();
	virtual bool isConnecting();

	virtual bool getMessage(std::string &message);
	virtual bool topMessage(std::string &message);

private:
	void write(NetworkPackagePtr msg);

	void handle_connect(const boost::system::error_code& error,
			tcp::resolver::iterator endpoint_iterator);

	void handle_read_header(const boost::system::error_code& error);

	void handle_read_body(const boost::system::error_code& error);

	void do_write(NetworkPackagePtr msg);

	void handle_write(const boost::system::error_code& error);

	void do_close();

	int id_;
	bool connected_;
	bool connecting_;
public:
	boost::asio::io_service io_service_;
private:
	boost::asio::ip::tcp::socket socket_;
	NetworkPackage read_msg_;
	NetworkPackagePtrQueue write_msgs_;
	std::deque<std::string> received_msqs_;
	boost::thread * t_;
	boost::mutex queueMutex_;
	boost::mutex writeQueueMutex_;
};

#endif /* NETWORKCLIENT_H_ */
