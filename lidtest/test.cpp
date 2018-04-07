#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>

#define PORT 23907
#define BUFSIZE 1024

using namespace std;
unsigned char buf[BUFSIZE];

int setupServer(int port) {
    int sock, bnd;
    struct sockaddr_in myaddr;
    memset((char *)&myaddr, 0, sizeof(myaddr));

    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
        return sock;
    if ((bnd = bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr))) < 0)
        return bnd;
    return sock;
}

int read(int fd, unsigned char *data) {
    return recv(fd, data, BUFSIZE, 0);
}

int main() {
    unsigned char *controls;
    cout << getprotobyname("udp");
    return 0;
}

