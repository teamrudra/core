#ifndef __UDP_H__
#define __UDP_H__
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <arpa/inet.h>

#define R_BUFSIZE 128
#define W_BUFSIZE 256
#define TIMEOUT 0
#define U_TIMEOUT 0

using namespace std;

class Udp {
    private:
        unsigned char buffer[R_BUFSIZE];
        int sock, bnd;
        struct sockaddr_in rover, ground;
        socklen_t addrlen;
        fd_set stReadFDS;
    public:
        Udp(int port);
        unsigned char* read();
        int write(unsigned char *data, int port);
};

#endif
