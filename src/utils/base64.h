/*
 * Base64 encoding/decoding (RFC1341)
 * Based upon http://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c
 *
 * See README for more details.
 */

#ifndef BASE64_H
#define BASE64_H
#include <string>
#include <vector>

namespace ImageServer {

namespace Utils {

class Base64 {
public:

static std::string encode(const unsigned char *src, size_t len);

static std::vector<unsigned char>
decode(const unsigned char *src, size_t len);

private:
    static unsigned char *decodeTable();

    static constexpr const unsigned char
    encodeTable[65] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

};

}

}

#endif /* BASE64_H */
