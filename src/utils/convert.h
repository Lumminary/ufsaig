/*
 * convert.h
 *
 *  Created on: Apr 15, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_CONVERT_H_
#define UTILS_CONVERT_H_

#include <string>
#include <vector>

namespace ImageServer {

namespace Utils {

bool toBool(const std::string& str);

std::string toLower(const std::string& str);

std::vector<std::string> split(const std::string& src, char sep);

}

}



#endif /* UTILS_CONVERT_H_ */
