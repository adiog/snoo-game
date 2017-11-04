/*
 * Utils.cpp
 *
 *  Created on: Apr 21, 2009
 *      Author: karol
 */

#include "Utils.h"

namespace network {

Utils::Utils() {

}

int Utils::splitStr(std::vector<std::string>& L, const std::string& seq, const std::string& _1cdelim, bool keeptoken) {
	typedef std::string::size_type ST;
	std::string delims = _1cdelim;
	std::string STR;
	if (delims.empty()) delims = "\n";

	ST pos = 0, LEN = seq.size();
	while (pos < LEN) {
		STR = ""; // Init/clear the STR token buffer
		// remove any delimiters including optional (white)spaces
		while ((delims.find(seq[pos]) != std::string::npos) && (pos < LEN))
			++pos;
		// leave if @eos
		if (pos == LEN) return L.size();
		// Save token data
		while ((delims.find(seq[pos]) == std::string::npos) && (pos < LEN))
			STR += seq[pos++];
		// put valid STR buffer into the supplied list
		//std::cout << "[" << STR << "]";
		if (!STR.empty()) L.push_back(STR);
	}
	return L.size();
}

}
