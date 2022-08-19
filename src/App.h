/*
 * ImageServerApp.h
 *
 *  Created on: Jan 26, 2021
 *      Author: Sergii Kutnii
 */

#ifndef IMAGESERVERAPP_H_
#define IMAGESERVERAPP_H_

#include <string>
#include "Config.h"
#include "services/ServiceDecl.h"

namespace ImageServer {

class App {
public:
    App(const std::string& configPath);
    void run();
private:
    Config m_config;
    Core::RootPtr m_main = nullptr;
    Uploads::RootPtr m_uploads = nullptr;
};

}

#endif /* IMAGESERVERAPP_H_ */
