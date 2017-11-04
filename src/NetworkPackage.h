/*
 * NetworkPackage.h
 *
 *  Created on: Nov 29, 2008
 *      Author: Karol Ruszczyk
 */

#ifndef NETWORKPACKAGE_H_
#define NETWORKPACKAGE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <sstream>
#include <string>

class NetworkPackage {
public:
	NetworkPackage(size_t dataSize, const void * data);
	NetworkPackage();
	virtual ~NetworkPackage();

	enum {
		max_input_data_size = 4096
	};

	enum {
		max_encrypted_data_size = max_input_data_size * 2 + 1
	};

	enum {
		header_length = 4
	};
	enum {
		info_length = 4
	};

	virtual void * getDecodedData() const;
	virtual size_t getDecodedDataSize() const;

	virtual std::string getDecodedDataAsString() const;

	virtual const char* message() const;
	virtual char* message();

	virtual const char* data() const;
	virtual char* data();

	virtual size_t messageSize() const;

	virtual size_t encryptedDataSize() const;

	virtual bool decodeHeader();

	virtual void encodeHeader();

private:
	/* data jest ciągiem bajtów o długości data_size (ciągiem
	 * bajtów a nie stringiem, nie ma żadnego 0 na końcu, może
	 * mieć dowolne bajty w środku).
	 * dst powinno miec co najmniej 2*src_size+1 bajtów
	 */
	void encodeData(const void *data, char *dst, size_t data_size);

	/* src jest stringiem wyprodukowanym przez encode,
	 * w data będzie zapisany pierwotny ciag bajtów, jego długość
	 * będzie równa strlen(src) / 2
	 */
	void decodeData(const char *src, void *data) const;

	//char* convertStringToChar(const string &str) const;
	char message_[max_encrypted_data_size + header_length];
	size_t encryptedDataSize_;

};

#endif /* NETWORKPACKAGE_H_ */
