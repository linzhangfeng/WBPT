#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>
#include <errno.h>

#include "network.h"


#if defined(__ANDROID__)
#define SO_NOSIGPIPE MSG_NOSIGNAL
#endif

static int s_nFlag = 1;
static bool s_bIpv6Enable = false;

Network::Network()
{

}

Network::~Network()
{

}

void Network::setIpv6Enable(bool enable)
{
    s_bIpv6Enable = enable;
}

/* below code copy from UNP v1 source code */
int Network::connect_nonb(int sockfd, const struct sockaddr *saptr, socklen_t salen, int nsec)
{
    int flags, n, error;
    socklen_t   len;
    fd_set  rset, wset;
    struct timeval  tval;

    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    error = 0;
    if ((n = connect(sockfd, (struct sockaddr *) saptr, salen)) < 0) {
        if (errno != EINPROGRESS) {
            goto done;
        }
    }
    /* Do whatever we want while the connect is taking place. */
    if (n == 0)
        goto done;    /* connect completed immediately */

    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tval.tv_sec = nsec;
    tval.tv_usec = 0;

    if ( (n = select(sockfd+1, &rset, &wset, NULL,
                     nsec ? &tval : NULL)) == 0) {
        close(sockfd);        /* timeout */
        errno = ETIMEDOUT;
        goto done;
    }

    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
        len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
            goto done;            /* Solaris pending error */
        }
    } else
        cout << "Unable to do select because of " << strerror(errno) << endl;

done:
    fcntl(sockfd, F_SETFL, flags);    /* restore file status flags */

    if (error) {
        close(sockfd);        /* just in case */
        errno = error;
        return(-1);
    }
    return(0);
}

int Network::tcp_connect(const char*ip, int port, int sec)
{
    if (s_bIpv6Enable)
    {
        char buf[32];
        char ip_buf[128];
        snprintf(buf, sizeof(buf), "%d",port);
        struct addrinfo hints, *res, *res0;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = PF_INET6;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_DEFAULT;
        getaddrinfo(ip, buf, &hints, &res0);
    
        int ret;
        int fd;
    //    unsigned long inaddr = 0;
    //    struct hostent *host;
        struct sockaddr_in6 addr;
    
        fd = socket(AF_INET6, SOCK_STREAM, 0);
        if (fd < 0) {
            cout << "Unable to open socket because of " << strerror(errno) << endl;
            return -1;
        }
        
        if (++s_nFlag > 10000)
        {
            s_nFlag = 0;
        }
        
        if (s_nFlag % 2)
        {
            snprintf(ip_buf, sizeof(ip_buf),"::FFFF:%s",ip);
        }
        else
        {
            snprintf(ip_buf, sizeof(ip_buf),"64:ff9b::%s",ip);
        }
    
        memset(&addr, 0, sizeof(struct sockaddr_in6));
        addr.sin6_family = AF_INET6;
        addr.sin6_port = htons(port);
        inet_pton(AF_INET6, ip_buf, &addr.sin6_addr);
        
        ret = connect_nonb(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in6), sec);
        
        if (ret < 0) {
            cout << "Unable to connect " << ip << ":" << port << " socket because of " << strerror(errno) << endl;
            close(fd);
            return -1;
        }
        //cout << "Connect to " << inet_ntoa(addr.sin_addr) << " OK." << endl;
        int set =1;
        setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&set, sizeof(int));
        return fd;
    }
    else
    {
        int ret;
        int fd;
        unsigned long inaddr = 0;
        struct hostent *host;
        struct sockaddr_in addr;
        
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            cout << "Unable to open socket because of " << strerror(errno) << endl;
            return -1;
        }
        
        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inaddr = inet_addr(ip);
        if (inaddr == INADDR_NONE) {
            host = gethostbyname(ip);
            if (host == NULL) {
                perror("gethostbyname");
                close(fd);
                return -1;
            }
            memcpy((char*)&addr.sin_addr, host->h_addr, host->h_length);
        } else
            addr.sin_addr.s_addr = inaddr;
        
        ret = connect_nonb(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in),sec);
        if (ret < 0) {
            cout << "Unable to connect " << ip << ":" << port << " socket because of " << strerror(errno) << endl;
            close(fd);
            return -1;
        }
        //cout << "Connect to " << inet_ntoa(addr.sin_addr) << " OK." << endl;
        int set =1;
        setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&set, sizeof(int));
        return fd;
    }
}

int Network::tcp_connect(const char* ip, int port)
{
    int ret;
    int fd;
    unsigned long inaddr = 0;
    struct hostent *host;
    struct sockaddr_in addr;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        cout << "Unable to open socket because of " << strerror(errno) << endl;
        return -1;
    }
    
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inaddr = inet_addr(ip);
    if (inaddr == INADDR_NONE) {
        host = gethostbyname(ip);
        if (host == NULL) {
            perror("gethostbyname");
            close(fd);
            return -1;
        }
        memcpy((char*)&addr.sin_addr, host->h_addr, host->h_length);
    } else
        addr.sin_addr.s_addr = inaddr;
    
    ret = connect(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        cout << "Unable to connect " << ip << ":" << port << " socket because of " << strerror(errno) << endl;
        close(fd);
        return -1;
    }
    //cout << "Connect to " << inet_ntoa(addr.sin_addr) << " OK." << endl;
    
    return fd;
}

int Network::tcp_connect(const char*ip, int port, int sec, int cnt)
{
    int ret;
    
    while (1)
    {
        ret = Network::tcp_connect(ip, port, sec);
        if (ret < 0)
        {
            cnt--;
            if (cnt < 0)
            {
                return -1;
            }
            sleep(2);
        }
        else
        {
            return ret;
        }
    }
}
