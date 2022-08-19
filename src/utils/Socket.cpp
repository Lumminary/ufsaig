/*
 * Socket.cpp
 *
 *  Created on: Feb 17, 2021
 *      Author: Sergii Kutnii
 */

#include "Socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdexcept>
#include <string>

namespace ImageServer {

namespace Utils {

//------------------------------------------------------------------------------

Socket::Socket(unsigned int port) {
    m_addr = socket(AF_INET, SOCK_STREAM, 0);
    if (m_addr == -1)
        throw std::runtime_error{ "Failed to create socket endpoint" };

    struct sockaddr_in server_addr = { 0 };
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);
    if(-1 == bind(m_addr,
            reinterpret_cast<sockaddr*>(&server_addr), sizeof(sockaddr_in)))
        throw std::runtime_error{
            "Could not bind to port " + std::to_string(port) };

    if (-1 == listen(m_addr, 1))
        throw std::runtime_error{
            "Could not listen on port " + std::to_string(port) };
}

//------------------------------------------------------------------------------

Socket::~Socket() {
    close(m_addr);
}

}
/* namespace Utils */
} /* namespace ImageServer */
