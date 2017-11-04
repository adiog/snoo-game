/*
 * NetworkClient.cpp
 *
 *  Created on: Nov 15, 2008
 *      Author: Karol Ruszczyk
 */

#include "NetworkClient.h"

#include <string>

#include <sstream>

using boost::asio::ip::tcp;

NetworkClient::NetworkClient(int id) :
	id_(id), connected_(false), connecting_(false), socket_(io_service_) {
}

NetworkClient::NetworkClient() :
	connected_(false), connecting_(false), socket_(io_service_) {
	id_ = -1;
}

NetworkClient::~NetworkClient() {
}

void NetworkClient::establishConnection(const char *server, const char *port) {
	tcp::resolver resolver(io_service_);
	tcp::resolver::query query(server, port);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	tcp::endpoint endpoint = *endpoint_iterator;
	socket_.async_connect(endpoint, boost::bind(&NetworkClient::handle_connect, this,
												boost::asio::placeholders::error,
												++endpoint_iterator));

	connecting_ = true;

	t_ = new boost::thread((boost::bind(&boost::asio::io_service::run, &io_service_)));

}

int NetworkClient::messagesWaiting() {
	boost::mutex::scoped_lock lock(queueMutex_);
	return received_msqs_.size();
}

bool NetworkClient::getMessage(std::string &message) {
	boost::mutex::scoped_lock lock(queueMutex_);
	if (received_msqs_.empty()) return false;
	message = received_msqs_.back();
	received_msqs_.pop_back();
	return true;

}

bool NetworkClient::topMessage(std::string &message) {
	boost::mutex::scoped_lock lock(queueMutex_);
	if (received_msqs_.empty()) return false;
	message = received_msqs_.back();
	return true;

}

void NetworkClient::onMessageReceived() {
	// TODO To nigdy nie jest uzywane.. i chyba nie ma za bardzo jak tego uzyc jezeli nie dziedziczy sie z NetworkClient
}

bool NetworkClient::send(size_t size, const void * data) {
	NetworkPackagePtr msg(new NetworkPackage(size, data));
	this->write(msg);
	return true;
}

void NetworkClient::disconnect() {
	//io_service_.post(boost::bind(&NetworkClient::do_close, this));
	io_service_.stop();
	this->do_close();
}

void NetworkClient::write(NetworkPackagePtr msg) {
	io_service_.post(boost::bind(&NetworkClient::do_write, this, msg));
}

void NetworkClient::handle_connect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator) {
	if (!error) {
		//std::cerr << "Polaczony!" << std::endl;
		connected_ = true;
		connecting_ = false;
		boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.message(),
																NetworkPackage::header_length),
								boost::bind(&NetworkClient::handle_read_header, this,
											boost::asio::placeholders::error));
	} else if (endpoint_iterator != tcp::resolver::iterator()) {
		socket_.close();
		tcp::endpoint endpoint = *endpoint_iterator;
		socket_.async_connect(endpoint, boost::bind(&NetworkClient::handle_connect, this,
													boost::asio::placeholders::error,
													++endpoint_iterator));
	} else {
		connecting_ = false;
	}
}

void NetworkClient::handle_read_header(const boost::system::error_code& error) {
	if (!error && read_msg_.decodeHeader()) {
		boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(),
																read_msg_.encryptedDataSize()),
								boost::bind(&NetworkClient::handle_read_body, this,
											boost::asio::placeholders::error));
	} else {
		do_close();
	}
}

void NetworkClient::handle_read_body(const boost::system::error_code& error) {
	if (!error) {
		char * data = (char*) read_msg_.getDecodedData();

		std::string s(data, read_msg_.getDecodedDataSize());

		boost::mutex::scoped_lock lock(queueMutex_);
		received_msqs_.push_front(s);

		delete [] data;

		this->onMessageReceived();

		boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.message(),
																NetworkPackage::header_length),
								boost::bind(&NetworkClient::handle_read_header, this,
											boost::asio::placeholders::error));
	} else {
		do_close();
	}
}

void NetworkClient::do_write(NetworkPackagePtr msg) {
	boost::mutex::scoped_lock lock(queueMutex_);
	bool write_in_progress = !write_msgs_.empty();
	write_msgs_.push_back(msg);
	//std::cerr << "do_write" << std::endl;
	if (!write_in_progress) {
		boost::asio::async_write(socket_,
									boost::asio::buffer(write_msgs_.front()->message(),
														write_msgs_.front()->messageSize()),
									boost::bind(&NetworkClient::handle_write, this,
												boost::asio::placeholders::error));
	}
}

void NetworkClient::handle_write(const boost::system::error_code& error) {
	if (!error) {
		boost::mutex::scoped_lock lock(queueMutex_);
		write_msgs_.pop_front();
		if (!write_msgs_.empty()) {
			boost::asio::async_write(socket_,
										boost::asio::buffer(write_msgs_.front()->message(),
															write_msgs_.front()->messageSize()),
										boost::bind(&NetworkClient::handle_write, this,
													boost::asio::placeholders::error));
		}
	} else {
		do_close();
	}
}

bool NetworkClient::isConnected() {
	return connected_;
}
bool NetworkClient::isConnecting() {
	return connecting_;
}

void NetworkClient::do_close() {
	//	std::cerr << "NetworkClient::do_close --> t_->join()" << std::endl;
	t_->join();
	if (socket_.is_open()) {
		socket_.cancel();
		socket_.close();
	}
	std::cerr << "NetworkClient::do_close --> end" << std::endl;
}
