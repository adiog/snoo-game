/*
 * NetworkPackage.cpp
 *
 *  Created on: Nov 29, 2008
 *      Author: Karol Ruszczyk
 */

#include "NetworkPackage.h"

NetworkPackage::NetworkPackage(size_t dataSize, const void * data) :
	encryptedDataSize_(dataSize * 2 + 1) {
	this->encodeData(data, this->data(), dataSize);
	this->encodeHeader();
}

NetworkPackage::NetworkPackage() {
	//std::cout << "msg jest tworzona - pusta" << std::endl;
}

NetworkPackage::~NetworkPackage() {
	//std::cout << "msg jest niszczona" << std::endl;
}

const char* NetworkPackage::message() const {
	return message_;
}

char* NetworkPackage::message() {
	return message_;
}

const char* NetworkPackage::data() const {
	return message_ + header_length;
}

char* NetworkPackage::data() {
	return message_ + header_length;
}

size_t NetworkPackage::messageSize() const {
	return encryptedDataSize_ + header_length;
}

size_t NetworkPackage::encryptedDataSize() const {
	return encryptedDataSize_;
}

void* NetworkPackage::getDecodedData() const {
	char * obj = new char[this->encryptedDataSize() / 2];
	this->decodeData(data(), (void*) obj);

	return (void*) obj;
}

size_t NetworkPackage::getDecodedDataSize() const {
	return this->encryptedDataSize() / 2;
}

//Kompatybilnosc wstecz..
std::string NetworkPackage::getDecodedDataAsString() const {
	char * obj = new char[this->encryptedDataSize() / 2 + 1];
	this->decodeData(data(), (void*) obj);
	obj[this->encryptedDataSize() / 2] = '\0';
	std::string s(obj);
	std::cerr << "NetworkPackage::getDecodedDataAsString --> Zwracany string to (" << s << ")" << std::endl;
	return s;
}

bool NetworkPackage::decodeHeader() {
	char info1[info_length + 1] = "";
	std::strncat(info1, message_, info_length);
	encryptedDataSize_ = std::atoi(info1);
	if (encryptedDataSize_ > max_encrypted_data_size) {
		encryptedDataSize_ = 0;
		return false;
	}
	return true;
}

void NetworkPackage::encodeHeader() {
	char header[header_length + 1] = "";
	std::sprintf(header, "%4d", (int) encryptedDataSize_);
	std::memcpy(message_, header, header_length);
}

void NetworkPackage::encodeData(const void *data, char *dst, size_t data_size) {
	int i;
	unsigned char *src = (unsigned char*) data;
	for (i = 0; i < (signed int) data_size; i++) {
		dst[2 * i] = ((src[i] & 0xF0) >> 4) | 64;
		dst[2 * i + 1] = (src[i] & 0x0F) | 64;
	}
	dst[2 * i] = 0; //null na koncu stringa
}

void NetworkPackage::decodeData(const char *src, void *data) const {
	unsigned char *dst = (unsigned char*) data;
	int i;
	int n = strlen(src) / 2;
	for (i = 0; i < n; i++)
		dst[i] = ((src[2 * i] & 0x0F) << 4) | (src[2 * i + 1] & 0x0F);
}

