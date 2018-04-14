#include "helper/helper.h"
#include "hmc5883l/hmc5883l.h"
#include "udp/udp.h"
#include "spi/spi.h"
#include "autobot/autobot.h"
#include <wiringSerial.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#define DRIVE 1
#define ARM 0
#define speed 1000000
#define port 3301

Spi drive(DRIVE,speed);
Spi arm(ARM, speed);
HMC5883L compass;
Udp gs(port);
Helper H;
Autobot A;

int dat,isAuto,size;
unsigned char data;
unsigned char *coods;

//double destlat = 12.821186, destlon = 80.038238;
double destlat[6],destlon[6];
double heading;

void kill() {
  cout<<"Killed";
  static unsigned char controls[2] = { (char)0, (char)0 };
  drive.RW(&controls[0],1);
  arm.RW(&controls[1], 1);
}


void setup() {
    dat = 0,isAuto = 0,size = 0;
//    coods = NULL;
    if(!H.gpsdintialise())
     cout<<"ERROR: initialising gpsd! "<<endl;
    if( hmc5883l_init(&compass) != HMC5883L_OKAY )
      cout << "ERROR: initialising compass!" << endl;

//    kill();
}

double Compass(double heading) {
  heading = compass._data.orientation_deg ;
  heading = H.maps(heading,0,360,360,0) - 90;
  heading = (heading<0)?heading + 360:heading;
  unsigned char* head = (unsigned char*)to_string(heading).c_str();
  gs.write(head,port);
  return heading;
}

void removeCoods(){
  for(int i = 1;i < size;i++){
    destlat[i-1] = destlat[i];
    destlon[i-1] = destlon[i];
  }
  size--;
}


void autonomous() {
  hmc5883l_read(&compass);
  heading = Compass(heading);
  A.destlat = destlat[0],A.destlon = destlon[0];
  //A.destlat = 12.821186,A.destlon = 80.038238;
  A.update(heading,H);
  if(dat == -1){
    removeCoods();
    //delay()
  }
  else {
    data = (unsigned char)dat;
    drive.RW(&data,1);
  }
}

int parseCoods(unsigned char *coods) {
  static int i, j = 0, start = 1, end;
  for (i = 1, j = 0; coods[i]!='$'; i++) {
    if(coods[i] == ',') {
      end = i;
      destlat[j] = H.parse_C_to_F(coods,start,end);
      start = end + 1;
    }
    else if(coods[i] == '!'){
      end = i;
      destlon[j] = H.parse_C_to_F(coods,start,end);
      start = end + 1;
      j++;
    }
  }
  return j;
}

void keyboard(unsigned char *coods) {
  static int i, mid;
  static unsigned char controls[2];
  for (i = 0; coods[i]!='>'; i++)
    mid = (coods[i] == ',') ? i : mid;
  controls[0] = H.parse(coods, 1, mid);
  controls[1] = H.parse(coods, mid+1, i);
  drive.RW(&controls[0],1);
  arm.RW(&controls[1], 1);
}

void check(unsigned char* coods) {
  isAuto = (coods[0] == '$')? 2 : isAuto;
  isAuto = (coods[0] == '#')? 1 : isAuto;
  isAuto = (coods[0] == '<')? 0 : isAuto;
  if (isAuto == 2 && size > 0 ) {
    removeCoods();
    cout << "Autonomous Stopped" << endl;
  }
  else if(isAuto == 1){
    size = parseCoods(coods);
    cout << "Autonomous running" << endl;
    autonomous();
  }
  else if(isAuto == 0){
    cout << "Keyboard running" << endl;
    keyboard(coods);
  }
}


void loop() {
  coods = gs.read();
  if(coods == NULL) kill();
  else check(coods);
}

int main() {
    setup();
    while (1) loop();
    return 0;
}

