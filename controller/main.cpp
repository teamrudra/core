#include "udp/udp.h"
#include "hmc5883l/hmc5883l.h"
#include "spi/spi.h"
#include <wiringSerial.h>
#include <iostream>
#include <fstream>
#include <string>

#define GPS_PORT "/dev/ttyUSB0"
HMC5883L compass;
string data;
int gps;

void setup() {
    if( hmc5883l_init(&compass) != HMC5883L_OKAY ) 
        cout << "ERROR: initialising compass!" << endl;
    gps = serialOpen ((char*)GPS_PORT, 9600) ;
    // gps.open(GPS_PORT);
}

void loop() {
    hmc5883l_read(&compass);
    // gps >> data;
    // cout << hmc5883l._data.orientation_deg << endl;
    cout << (char)serialGetchar (gps);
}

int main() {
    setup();
    while (1) loop();
    return 0;
}
