/*
 * VersionDescriptor.h
 *
 *  Created on: Apr 23, 2021
 *      Author: Sergii Kutnii
 */

#ifndef IMAGE_CONTROILLER_VERSIONDESCRIPTOR_H_
#define IMAGE_CONTROILLER_VERSIONDESCRIPTOR_H_

#include <string>

namespace ImageServer {

struct ImageDescriptor {
    std::string imageKey;
    unsigned int width = 0;
    unsigned int height = 0;
    std::string type;
    bool save = false;
    bool original = false;
    bool base64 = false;
};

} /* namespace ImageServer */

#endif /* IMAGE_CONTROILLER_VERSIONDESCRIPTOR_H_ */
