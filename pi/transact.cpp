#include <iostream>
#include <wiringPiSPI.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>

#define PORT 3301
#define BUFSIZE 1024
#define DRIVE 0
#define ARM 1
#define SPEED 1000000

// TODO: Ubiquity API

using namespace std;
unsigned char buf[BUFSIZE];

int spiSetup(int spiChannel) {
    int fd;
    if ((fd = wiringPiSPISetup (spiChannel, SPEED)) < 0){
        cerr << "Can't open the SPI bus on channel : " << spiChannel << endl;
        return fd;
    }
    return fd;
}

int getData(int spiChannel, unsigned char *data, int size) {
    int d;
    if ((d = wiringPiSPIDataRW (spiChannel, data, size)) < 0) {
        cerr << "SPI failure on channel : " << spiChannel << endl;
        return d;
    }
    return d;
}

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

unsigned char parse(unsigned char n[], int start, int end) {
    int a = 0;
    for (int i = start; i<end; i++) {
        a += (n[i]-'0');
        a *= 10;
    }
    return (unsigned char)(a/10);
}

int read(int fd, unsigned char *controls) {
    int recvlen, mid = 0, i = 0;
    static struct sockaddr_in remaddr;
    static socklen_t addrlen = sizeof(remaddr); 
    recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
    for (i = 0; buf[i]!='>'; i++) {
        // cout << buf[i];
        mid = (buf[i] == ',') ? i : mid;
    }
    // cout << endl;
    controls[0] = parse(buf, 1, mid);
    controls[1] = parse(buf, mid+1, i);
    return recvlen;
}

int main() {
    unsigned char controls[2];
    int bytes;
    int sock = setupServer(PORT);
    int driveFD = spiSetup(DRIVE);
   // int armFD = spiSetup(ARM);
    while (1) {
	read(sock, controls);
        bytes = getData(DRIVE, &controls[0], 1);
        // bytes = getData(ARM, &controls[1], 1);
        // cout << "drive = " << controls[0] << " arm = " << controls[1] << " bytes = " << bytes << endl;
    }
    return 0;
}

