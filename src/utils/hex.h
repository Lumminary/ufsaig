/*
 * hex.h
 *
 *  Created on: Jan 27, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_HEX_H_
#define UTILS_HEX_H_

#include <string>
#include <uuid/uuid.h>

namespace ImageServer {

namespace Utils {

std::string hexify(const uuid_t uuid);

std::string hexify(const unsigned char *bytes, size_t length);

}

}

#endif /* UTILS_HEX_H_ */
