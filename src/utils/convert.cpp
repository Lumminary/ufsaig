/*
 * convert.cpp
 *
 *  Created on: Apr 15, 2021
 *      Author: Sergii Kutnii
 */

#include "convert.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>


namespace ImageServer {

namespace Utils {

//------------------------------------------------------------------------------

bool toBool(const std::string& str) {
    std::istringstream is(toLower(str));
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

//------------------------------------------------------------------------------

std::string toLower(const std::string &str) {
    std::string lower{ str };
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

//------------------------------------------------------------------------------

std::vector<std::string> split(const std::string& src, char sep) {
    std::vector<std::string> result;
    size_t start = 0;
    size_t end = start;
    do {
        end = src.find_first_of(sep, start);
        if (end == std::string::npos)
            break;

        result.push_back(src.substr(start, end - start));
        start = end + 1;
    } while(true);

    if (start < src.size())
        result.push_back(src.substr(start));

    return result;
}

}


}


