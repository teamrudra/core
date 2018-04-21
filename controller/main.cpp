#include "helper/helper.h"
#include "hmc5883l/hmc5883l.h"
#include "udp/udp.h"
#include "spi/spi.h"
#include <wiringSerial.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#define DRIVE 0
#define ARM 1
#define speed 1000000
#define ACCURACY 1

Spi drive(DRIVE,speed);
Spi arm(ARM, speed);
HMC5883L compass;
Helper H;
Udp gs(3301);

unsigned char data;
unsigned char *coods;

int Right = 16,Left = 64,Leftdrift = 192,Rightdrift = 144,fwd = 128,stop = 0;
int err = 5,dat = 0, isAuto = 0;

double destlat = 12.821186, destlon = 80.038238;
double latitude,longitude;
double heading,bearing,dist;

void setup() {
    if(!H.gpsdintialise())
       cout<<"ERROR: initialising gpsd! "<<endl;
    if( hmc5883l_init(&compass) != HMC5883L_OKAY )
      cout << "ERROR: initialising compass!" << endl;
    dat = 0;
    data = (unsigned char)dat;
    drive.RW(&data,1);
    arm.RW(&data, 1);
}

double Compass(double heading) {
  heading = compass._data.orientation_deg ;
  heading = H.maps(heading,0,360,360,0) - 90;
  return heading =  (heading<0)?heading + 360:heading;
}

void autodrive(double diff,double dist) {
    if( dist < ACCURACY) dat = stop;
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
    drive.RW(&data,1);
}

void autobot(unsigned char *coods) {
  // parse location points
  // queuing logic
  hmc5883l_read(&compass);
  R.get_latlon(latitude, longitude);
  heading = Compass(heading);
  bearing = R.get_bearing(latitude,longitude,destlat,destlon);
  dist = R.get_dist(latitude,longitude,destlat,destlon);
  cout <<"Bearing: "<<bearing<<" Heading: "<<heading<<" Distance: "<<dist;
  double diff = heading - bearing;
  if(abs(diff)>180){
    diff = (360-abs(diff))*diff/abs(diff);
    autodrive(-diff,dist);
  }
  else autodrive(diff,dist);
}

void keyboard(unsigned char *coods) {
  static int i, mid;
  static unsigned char controls[2];
  for (i = 0; coods[i]!='>'; i++)
    mid = (coods[i] == ',') ? i : mid;
  controls[0] = R.parse(coods, 1, mid);
  controls[1] = R.parse(coods, mid+1, i);
  drive.RW(&controls[0],1);
  arm.RW(&controls[1], 1);
}

void kill() {
  static unsigned char controls[2] = { (char)0, (char)0 };
  drive.RW(&controls[0],1);
  arm.RW(&controls[1], 1);
}

void check(unsigned char* coods) {
  isAuto = (coods[0] == '#' && !isAuto ? 1 : isAuto);
  isAuto = (coods[0] == '$' && isAuto ? 0 :  isAuto);
  if (isAuto) {
    cout << "Autonomous running" << endl;
  } else {
    cout << "Keyboard running" << endl;
  }
  cout << coods << endl;
}


void loop() {
  coods = gs.read();
  check(coods);
}

int main() {
    setup();
    while (1) loop();
    return 0;
}
