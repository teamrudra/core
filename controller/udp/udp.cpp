#include "udp.h"

Udp::Udp(int port) {
    addrlen = sizeof(ground);
    memset((char *)&rover, 0, sizeof(rover));
    rover.sin_family = AF_INET;
    rover.sin_addr.s_addr = htonl(INADDR_ANY);
    rover.sin_port = htons(port);
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        cout << "ERROR: setting socket in UDP" << endl;
    }
    if ((bnd = bind(sock, (struct sockaddr *)&rover, sizeof(rover))) < 0) {
        cout << "ERROR: binding server in UDP" << endl;
    }
}

unsigned char* Udp::read() {
    static struct timeval stTimeOut;
    stTimeOut.tv_sec = TIMEOUT;
    stTimeOut.tv_usec = U_TIMEOUT;
    fd_set stReadFDS;
    FD_ZERO(&stReadFDS);
    FD_SET(sock, &stReadFDS);
    if (select(sock + 1, &stReadFDS, NULL, NULL, &stTimeOut) < 0) {
        cout << "ERROR: setting timeout function in UDP" << endl;
    }
    if (FD_ISSET(sock, &stReadFDS) && 
        recvfrom(sock, buffer, R_BUFSIZE, 0, (struct sockaddr *)&ground, &addrlen) > 0) {
        return buffer;
    } else {
        return (unsigned char*)"NULL";
    }
}

int Udp::write(unsigned char *data, int port) {
    ground.sin_port = htons(port);
    return sendto(sock, data, W_BUFSIZE, 0, (struct sockaddr*)&ground, sizeof(ground));
}

