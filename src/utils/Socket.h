/*
 * Socket.h
 *
 *  Created on: Feb 17, 2021
 *      Author: Sergii Kutnii
 */

#ifndef UTILS_SOCKET_H_
#define UTILS_SOCKET_H_

namespace ImageServer {

namespace Utils {

class Socket {
public:
    Socket(unsigned int port);

    int addr() const { return m_addr; }

    ~Socket();
private:
    int m_addr;
};

} /* namespace Utils */

} /* namespace ImageServer */

#endif /* UTILS_SOCKET_H_ */
