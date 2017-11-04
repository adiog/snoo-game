/*
 * NetworkSession.h
 *
 *  Created on: Nov 15, 2008
 *      Author: Karol Ruszczyk
 */

#ifndef NETWORKSESSION_H_
#define NETWORKSESSION_H_

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <deque>
#include <set>

#include "NetworkPackage.h"
#include "NetworkServer.h"

using boost::asio::ip::tcp;

typedef std::deque<NetworkPackage> NetworkPackageQueue;

class NetworkServer;

class NetworkSession: public boost::enable_shared_from_this<NetworkSession> {
public:
	NetworkSession();
	virtual ~NetworkSession();

	NetworkSession(boost::asio::io_service& io_service, NetworkServer& server, int id);

	tcp::socket& socket();

	void start();

	void deliver(const NetworkPackage& msg);

	void handle_read_header(const boost::system::error_code& error);

	void handle_read_body(const boost::system::error_code& error);

	void handle_write(const boost::system::error_code& error);

private:
	tcp::socket socket_;
	NetworkServer& server_;
	NetworkPackage read_msg_;
	NetworkPackageQueue write_msgs_;

	boost::mutex queueMutex_;

	int id_;
};

#endif /* NETWORKSESSION_H_ */
