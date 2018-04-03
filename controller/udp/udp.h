#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>

#define BUFSIZE 2048
#define TIMEOUT 1
#define U_TIMEOUT 0

using namespace std;

class Udp {
    private:
        unsigned char buf[BUFSIZE];
        int sock, bnd;
        struct sockaddr_in myaddr;
        struct sockaddr_in remaddr;
        socklen_t addrlen;
    public:
        Udp(int port);
        unsigned char* read();
        int write(unsigned char *data);
};
