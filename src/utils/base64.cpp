/*
 * Base64 encoding/decoding (RFC1341)
 * Based upon http://web.mit.edu/freebsd/head/contrib/wpa/src/utils/base64.c
 *
 * See README for more details.
 */

#include "base64.h"
#include <string>
#include <vector>
#include <mutex>

namespace ImageServer {

namespace Utils {

//------------------------------------------------------------------------------

unsigned char*
Base64::decodeTable() {
    static std::vector<unsigned char> table;
    static std::once_flag initFlag;

    std::call_once(initFlag, [&]() {
        table.resize(256, 0x80);
        for (size_t i = 0; i < sizeof(encodeTable) - 1; i++)
            table[encodeTable[i]] = (unsigned char) i;
        table['='] = 0;
    });

    return table.data();
}

//------------------------------------------------------------------------------

std::string
Base64::encode(const unsigned char *src,
        size_t len) {

    unsigned char *pos;
    const unsigned char *end, *in;
    size_t olen;

    olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
    if (olen < len)
        return ""; /* integer overflow */

    std::string out;
    out.resize(olen, ' ');

    end = src + len;
    in = src;
    pos = (unsigned char*)out.c_str();
    while (end - in >= 3) {
        *pos++ = encodeTable[in[0] >> 2];
        *pos++ = encodeTable[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = encodeTable[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = encodeTable[in[2] & 0x3f];
        in += 3;
    }

    if (end - in) {
        *pos++ = encodeTable[in[0] >> 2];
        if (end - in == 1) {
            *pos++ = encodeTable[(in[0] & 0x03) << 4];
            *pos++ = '=';
        } else {
            *pos++ = encodeTable[((in[0] & 0x03) << 4) |
                          (in[1] >> 4)];
            *pos++ = encodeTable[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
    }

    return out;
}

//------------------------------------------------------------------------------

std::vector<unsigned char>
Base64::decode(const unsigned char *src, size_t len) {
    unsigned char block[4], tmp;
    int pad = 0;

    unsigned char *dTable = decodeTable();

    size_t count = 0;
    for (size_t i = 0; i < len; i++) {
        if (dTable[src[i]] != 0x80)
            count++;
    }

    if (count == 0 || count % 4)
        return std::vector<unsigned char>{};

    size_t olen = count / 4 * 3;
    std::vector<unsigned char> out;
    out.resize(olen, '\0');
    unsigned char *pos = out.data();

    count = 0;
    for (size_t i = 0; i < len; i++) {
        tmp = dTable[src[i]];
        if (tmp == 0x80)
            continue;

        if (src[i] == '=')
            pad++;
        block[count] = tmp;
        count++;
        if (count == 4) {
            *pos++ = (block[0] << 2) | (block[1] >> 4);
            *pos++ = (block[1] << 4) | (block[2] >> 2);
            *pos++ = (block[2] << 6) | block[3];
            count = 0;
            if (pad) {
                if (pad == 1)
                    pos--;
                else if (pad == 2)
                    pos -= 2;
                else {
                    /* Invalid padding */
                    return std::vector<unsigned char>{};
                }
                break;
            }
        }
    }

    return out;
}


}

}
