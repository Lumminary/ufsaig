/*
 * Config.cpp
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#include "Config.h"

namespace Couchbase {

Config::Config(const Json::Value &options) {
    cluster = options["isCluster"].asBool();
    connStr = options["connectionString"].asString();
    user = options["username"].asString();
    password = options["password"].asString();
    bucket = options["bucket"].asString();
    timeout = options["timeoutMicrosec"].asInt();
}

} /* namespace Couchbase */
