/*
 * NetworkSession.cpp
 *
 *  Created on: Nov 15, 2008
 *      Author: Karol Ruszczyk
 */

#include "NetworkSession.h"

NetworkSession::~NetworkSession() {
}

NetworkSession::NetworkSession(boost::asio::io_service& io_service,
		NetworkServer& server, int id) :
	socket_(io_service), server_(server), id_(id) {
}

tcp::socket& NetworkSession::socket() {
	return socket_;
}

void NetworkSession::start() {
	boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.message(), NetworkPackage::header_length),
			boost::bind(&NetworkSession::handle_read_header, shared_from_this(),
			boost::asio::placeholders::error));
}

void NetworkSession::deliver(const NetworkPackage& msg) {
	boost::mutex::scoped_lock lock(queueMutex_);
	bool write_in_progress = !write_msgs_.empty();
	write_msgs_.push_back(msg);
	if (!write_in_progress) {
		boost::asio::async_write(socket_, boost::asio::buffer(
				write_msgs_.front().message(), write_msgs_.front().messageSize()),
				boost::bind(&NetworkSession::handle_write, shared_from_this(),
						boost::asio::placeholders::error));
	}
}

void NetworkSession::handle_read_header(const boost::system::error_code& error) {
	if (!error && read_msg_.decodeHeader()) {
		//std::cerr << "Naglowek to : " << read_msg_.sender() << " " << read_msg_.encryptedDataSize() << std::endl;
		boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(),
				read_msg_.encryptedDataSize()), boost::bind(
				&NetworkSession::handle_read_body, shared_from_this(),
				boost::asio::placeholders::error));
	} else {
		server_.disconnect(id_);
	}
}

void NetworkSession::handle_read_body(const boost::system::error_code& error) {
	if (!error) {
		//std::cerr << "NetworkSession::handle_read_body" << std::endl;
		server_.onDataReceived(id_, read_msg_);
		boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.message(),
				NetworkPackage::header_length), boost::bind(
				&NetworkSession::handle_read_header, shared_from_this(),
				boost::asio::placeholders::error));
	} else {
		server_.disconnect(id_);
	}
}

void NetworkSession::handle_write(const boost::system::error_code& error) {
	if (!error) {
		//std::cout << "NetworkSession - hw1" << std::endl;
		boost::mutex::scoped_lock lock(queueMutex_);
		write_msgs_.pop_front();
		if (!write_msgs_.empty()) {
			boost::asio::async_write(socket_, boost::asio::buffer(
					write_msgs_.front().message(), write_msgs_.front().messageSize()),
					boost::bind(&NetworkSession::handle_write,
							shared_from_this(),
							boost::asio::placeholders::error));
		}
	} else {
		server_.disconnect(id_);
	}
}
