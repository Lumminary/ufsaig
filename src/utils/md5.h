/*
 * md5.h
 *
 *  Created on: Feb 3, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_MD5_H_
#define UTILS_MD5_H_

#include <string>

namespace ImageServer {

namespace Utils {

std::string md5String(const unsigned char *data, size_t length);

}

}

#endif /* UTILS_MD5_H_ */
