/*
 * Config.h
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#ifndef COUCHBASE_CONFIG_H_
#define COUCHBASE_CONFIG_H_

#include <json/json.h>
#include <string>

namespace Couchbase {

struct Config {
    bool cluster = false;
    std::string connStr;
    std::string user;
    std::string password;
    std::string bucket;
    int timeout = -1; //Timeout in microseconds

    Config() = default;
    Config(const Json::Value& options);
    Config(const Config& other) = default;
};

} /* namespace Couchbase */

#endif /* COUCHBASE_CONFIG_H_ */
