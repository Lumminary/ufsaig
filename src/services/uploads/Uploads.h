/*
 * Root.h
 *
 *  Created on: Feb 2, 2021
 *      Author: Sergii Kutnii
 */

#ifndef SERVICES_UPLOAD_ROOT_H_
#define SERVICES_UPLOAD_ROOT_H_

#include "services/HttpService.h"
#include "repos/ImageStore.h"

namespace ImageServer {
namespace Uploads {

class Root;
using RootPtr = std::shared_ptr<Root>;

class Root : public HttpService {
public:
    ImageStorePtr imageStore;
    static RootPtr create(const std::string& resourcePath);
};

} /* namespace Upload */
} /* namespace ImageServer */

#endif /* SERVICES_UPLOAD_ROOT_H_ */
