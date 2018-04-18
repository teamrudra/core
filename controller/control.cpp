#include "udp/udp.h"
#include "spi/spi.h"
#include <iostream>

#define DRIVE 0
#define ARM 1
#define speed 1000000
#define port 3301

Spi drive(DRIVE,speed);
Spi arm(ARM, speed);
Udp gs(port);
unsigned char *data;

unsigned char parse(unsigned char *n, int start, int end) {
    int a = 0;
    for (int i = start; i<end; i++) {
        a += (n[i]-'0');
        a *= 10;
    }
    return (unsigned char)(a/10);
}

void keyboard(unsigned char *coods) {
    static int i, mid;
    static unsigned char controls[2];
    for (i = 0; coods[i]!='>'; i++)
        mid = (coods[i] == ',') ? i : mid;
    controls[0] = parse(coods, 1, mid);
    controls[1] = parse(coods, mid+1, i);
    // cout << controls[0] << ' ' << controls[1] << endl;
    drive.RW(&controls[0],1);
    arm.RW(&controls[1], 1);
}

int main() {
    while (1) {
        data = gs.read();
        if (data[0] == '<')
            keyboard(data);
        else
            keyboard((unsigned char*)"<0,0>");
    }
    return 0;
}
