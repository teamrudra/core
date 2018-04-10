#include "Rudra/rudra.h"
#include "hmc5883l/hmc5883l.h"
#include "spi/spi.h"
#include <wiringSerial.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#define channel 0
#define speed 1000000

Spi S1(channel,speed);
HMC5883L compass;
Rudra R;

unsigned char *data;
int size = 1;

double destlat = 12.821204, destlon = 80.038253;
double latitude,longitude;
double heading,bearing,dist;

void setup() {
    if(!R.gpsdintialise())
       cout<<"ERROR: initialising gpsd! "<<endl;
    if( hmc5883l_init(&compass) != HMC5883L_OKAY )
        cout << "ERROR: initialising compass!" << endl;
}

void loop() {
    hmc5883l_read(&compass);
    R.get_latlon(latitude,longitude);
    heading = compass._data.orientation_deg ;
    heading = R.maps(heading,0,360,360,0) - 88;
    heading =  (heading<0)?heading + 360:heading;
//    cout<<"Heading "<<heading<<endl;
    bearing = R.get_bearing(destlat,destlon,latitude,longitude);
    dist = R.get_dist(destlat,destlon,latitude,longitude);
    printf("Latitude: %.6f Longitude: %.6f ",latitude,longitude);
    cout <<"Bearing: "<<bearing<<" Heading"<<heading<<" Distance"<<dist<<endl;
}

int main() {
    setup();
    while (1) loop();
    return 0;
}

