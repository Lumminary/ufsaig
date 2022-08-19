/*
 * ServiceDecl.h
 *
 *  Created on: Jan 29, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_SERVICEDECL_H_
#define SERVICES_SERVICEDECL_H_

#include <memory>
#include <httpserver.hpp>

namespace ImageServer {

using Request = httpserver::http_request;
using RequestPtr = std::shared_ptr<Request>;

using Response = httpserver::http_response;
using ResponsePtr = std::shared_ptr<Response>;

using Resource = httpserver::http_resource;

namespace Core {

class Root;
using RootPtr = std::shared_ptr<Root>;

}

namespace Uploads {

class Root;
using RootPtr = std::shared_ptr<Root>;

}

}

namespace DB {

class Document;
using DocPtr = std::shared_ptr<Document>;

}

#endif /* SERVICES_SERVICEDECL_H_ */
