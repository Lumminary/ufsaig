/*
 * Config.cpp
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#include "Config.h"
#include <json/json.h>
#include <fstream>
#include <memory>
#include <sstream>

namespace ImageServer {

Config::Config(const std::string &path) {
    std::ifstream inStream{ path };

    std::stringstream buffer;
    buffer << inStream.rdbuf();

    std::string content{ buffer.str() };

    Json::Value root;
    Json::CharReaderBuilder readerFactory;
    std::unique_ptr<Json::CharReader> reader{ readerFactory.newCharReader() };

    const char *start = content.c_str();
    const char *end = start + content.size();
    std::string errs;
    if (!reader->parse(start, end, &root, &errs))
        throw std::runtime_error{ "Json error: " + errs };

    listenAddress = root["listen"].asString();
    port = root["port"].asInt();
    logPath = root["logPath"].asString();
    numThreads = root["serverThreads"].asInt();
    daemonize = root["daemonize"].asBool();
    logLevel = root["logLevel"].asInt();
    enableUploads = root["enableUploads"].asBool();
    maxRequestBytes = root["maxRequestBytes"].asInt();
    staticPath = root["staticPath"].asString();
    transformerThreadCount = root["transformerThreads"].asInt();
    renderingEngine = root["renderingEngine"].asString();

    metaDataSource = Couchbase::Config(root["metaDataSource"]);
    binaryDataSource = Couchbase::Config(root["binaryDataSource"]);
}

} /* namespace ImageServer */
