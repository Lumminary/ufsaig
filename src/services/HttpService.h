/*
 * Service.h
 *
 *  Created on: Feb 1, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_SERVICE_H_
#define SERVICES_SERVICE_H_

#include <httpserver.hpp>
#include <string>
#include <memory>

namespace ImageServer {

class HttpService {
public:
    virtual void attach(httpserver::webserver& engine,
            const std::string& prefix = "") = 0;

    virtual ~HttpService() = default;
};

using HttpServicePtr = std::shared_ptr<HttpService>;

} /* namespace ImageServer */

#endif /* SERVICES_SERVICE_H_ */
