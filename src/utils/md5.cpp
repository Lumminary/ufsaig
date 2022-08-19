/*
 * md5.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: Sergii Kutnii
 */

#include "md5.h"
#include <openssl/md5.h>
#include "hex.h"

namespace ImageServer {

namespace Utils {

std::string md5String(const unsigned char *data, size_t length) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(data, length, digest);

    return hexify(digest, MD5_DIGEST_LENGTH);
}


}

}

