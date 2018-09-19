#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <iostream>
#include <fstream>
#include <string>

#include <sys/socket.h>

using namespace std;

class Network {
public:
    Network();
    ~Network();
    
    static int connect_nonb(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec);
    static int tcp_connect(const char*ip, int port, int sec);
    static int tcp_connect(const char*ip, int port);
    static int tcp_connect(const char*ip, int port, int sec, int cnt);
    static void setIpv6Enable(bool enable);
};

#endif
