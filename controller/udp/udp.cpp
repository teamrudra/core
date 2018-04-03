#include "udp.h"

Udp::Udp(int port) {
    addrlen = sizeof(remaddr);
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cout << "ERROR: setting socket in UDP" << endl;
    }
    if ((bnd = bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr))) < 0) {
        cout << "ERROR: binding server in UDP" << endl;
    }
}

unsigned char* Udp::read() {
    int i = 0, mid = 0;
    static struct timeval stTimeOut;
    stTimeOut.tv_sec = TIMEOUT;
    stTimeOut.tv_usec = U_TIMEOUT;
    fd_set stReadFDS;
    FD_ZERO(&stReadFDS);
    FD_SET(sock, &stReadFDS);
    if (select(sock + 1, &stReadFDS, NULL, NULL, &stTimeOut) < 0) {
        cout << "ERROR: setting timeout function in UDP" << endl;
    }
    if (FD_ISSET(sock, &stReadFDS)) {
        if (recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen) < 0) {
            cout << "ERROR: reading data in UDP" << endl;
        }
        else {
            return buf;
        }
    } else {
        return (unsigned char*)"NULL";
    }
}

int Udp::write(unsigned char *data) {
    return sendto(sock, data, BUFSIZE, 0, (struct sockaddr*)&remaddr, sizeof(remaddr));
}

