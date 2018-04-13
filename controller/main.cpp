#include "Rudra/rudra.h"
#include "hmc5883l/hmc5883l.h"
#include "spi/spi.h"
#include <wiringSerial.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#define DRIVE 0
#define speed 1000000

Spi S1(DRIVE,speed);
HMC5883L compass;
Rudra R;

unsigned char data;
int size = 1;

int Right = 16,Left = 64,Leftdrift = 192,Rightdrift = 144,fwd = 128,stop = 0;
int err = 5,dat = 0;


double destlat = 12.821186, destlon = 80.038238;
double latitude,longitude;
double heading,bearing,dist;

void setup() {
    if(!R.gpsdintialise())
       cout<<"ERROR: initialising gpsd! "<<endl;
    if( hmc5883l_init(&compass) != HMC5883L_OKAY )
        cout << "ERROR: initialising compass!" << endl;
}

double Compass(double heading){
  heading = compass._data.orientation_deg ;
  heading = R.maps(heading,0,360,360,0) - 90;
  return heading =  (heading<0)?heading + 360:heading;
}

void autodrive(double diff,double dist){
    if(dist<2) dat = stop;
    else {
      if(diff > err) {
        if(dat == stop || dat == Left ) dat = Left;
        else if(dat == fwd ||dat == Leftdrift) dat = Leftdrift;
        else dat = stop;
      }else if(diff <- err){
        if(dat == stop || dat == Right ) dat = Right;
        else if(dat == fwd ||dat == Rightdrift) dat = Rightdrift;
        else dat = stop;
      }
      else{
        if(dat==Leftdrift || dat == Rightdrift || dat == fwd || dat == stop) dat = fwd;
        else dat = stop;
      }
    }
    cout<<"Diff: "<<diff<<" Dat: "<<dat<<endl;
    data = (unsigned char)dat;
    S1.RW(&data,1);
}

void autobot(double bear, double dist, double head)
{
  double diff = head - bear;

  if(abs(diff)>180){
    diff = (360-abs(diff))*diff/abs(diff);
    autodrive(-diff,dist);
  }
  else autodrive(diff,dist);
}


void loop() {
    hmc5883l_read(&compass);
    R.get_latlon(latitude,longitude);
    heading = Compass(heading);
    bearing = R.get_bearing(latitude,longitude,destlat,destlon);
    dist = R.get_dist(latitude,longitude,destlat,destlon);
    autobot(bearing,dist,heading);
//    printf("latitude %.6f longitude %.6f ",latitude,longitude);
    cout <<"Bearing: "<<bearing<<" Heading: "<<heading<<" Distance: "<<dist;
}

int main() {
//  try{
    setup();
    dat = 0;
    data = (unsigned char)dat;
    S1.RW(&data,1);
    while (1) loop();
    return 0;
//  }
//  catch (){
//	cout<<"Terminate";
//        dat = 0;
//        data = (unsigned char)dat;
//	S1.RW(&data,1);
//  }
}

