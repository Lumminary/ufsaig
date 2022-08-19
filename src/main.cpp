/*
 * main.cpp
 *
 *  Created on: Jan 22, 2021
 *      Author: Sergii Kutnii
 */

#include "App.h"
#include <iostream>

int main(int argc, const char **argv) {
    if (argc < 2) {
        std::cout << "Usage: ./image_server /path/to/config.json" << std::endl;
        return 0;
    }

    ImageServer::App{ argv[1] }.run();

    return 0;
}

