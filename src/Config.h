/*
 * Config.h
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include "couchbase/client.h"

namespace ImageServer {

struct Config {
    Config() = default;
    Config(const std::string& path);

    std::string listenAddress = "0.0.0.0";
    int port = 80;
    std::string logPath = "./";
    int numThreads = 1;
    bool daemonize = false;
    int logLevel = 0;
    bool enableUploads = false;
    int maxRequestBytes = 1024 * 1024;
    std::string staticPath = "";
    int transformerThreadCount = 1;
    std::string renderingEngine;

    Couchbase::Config metaDataSource;
    Couchbase::Config binaryDataSource;
};

} /* namespace ImageServer */

#endif /* CONFIG_H_ */
