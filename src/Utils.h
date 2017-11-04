/*
 * Utils.h
 *
 *  Created on: Apr 21, 2009
 *      Author: karol
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>

namespace network {

class Utils {
private:
	Utils();

public:
	//Utils
	static int splitStr(std::vector<std::string>& L, const std::string& seq, const std::string& _1cdelim, bool keeptoken = false);
};

}

#endif /* UTILS_H_ */
