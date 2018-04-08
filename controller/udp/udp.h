#ifndef __UDP_H__
#define __UDP_H__
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <arpa/inet.h>

#define BUFSIZE 2048
#define TIMEOUT 1
#define U_TIMEOUT 0

using namespace std;

class Udp {
    private:
        unsigned char buffer[BUFSIZE];
        int sock, bnd;
        struct sockaddr_in myaddr;
        struct sockaddr_in remaddr;
        socklen_t addrlen;
        fd_set stReadFDS;
    public:
        Udp(int port);
        unsigned char* read();
        int write(unsigned char *data);
};

#endif
