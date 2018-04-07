#include "udp.h"

Udp::Udp(int port) {
    addrlen = sizeof(remaddr);
    memset((char *)&remaddr, 0, sizeof(remaddr)); 
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        cout << "ERROR: setting socket in UDP" << endl;
    }
    if ((bnd = bind(sock, (struct sockaddr *)&myaddr, sizeof(myaddr))) < 0) {
        cout << "ERROR: binding server in UDP" << endl;
    }
}

unsigned char* Udp::read() {  
    if ( recvfrom(sock, buffer, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen) > 0) {
        return buffer;
    } else {
        return (unsigned char*)"NULL";
    }
}

int Udp::write(unsigned char *data) {
    remaddr.sin_port = htons(23907);
    cout << remaddr.sin_port;
    return sendto(sock, data, sizeof(data), 0, (struct sockaddr*)&remaddr, sizeof(remaddr));
}

