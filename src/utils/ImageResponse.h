/*
 * ImageResponse.h
 *
 *  Created on: Feb 18, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_IMAGERESPONSE_H_
#define UTILS_IMAGERESPONSE_H_

#include <httpserver.hpp>
#include "DataProvider.h"

namespace ImageServer {

namespace Utils {

std::shared_ptr<httpserver::http_response>
imageResponse(DataProviderPtr dataSource, const std::string& contentType);

}

}


#endif /* UTILS_IMAGERESPONSE_H_ */
