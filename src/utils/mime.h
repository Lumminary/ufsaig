/*
 * mime.h
 *
 *  Created on: Feb 4, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_MIME_H_
#define UTILS_MIME_H_

#include <string>

namespace ImageServer {

namespace Utils {

std::string mimeType(const std::string& format, bool applyLowercase = false);

}

}


#endif /* UTILS_MIME_H_ */
